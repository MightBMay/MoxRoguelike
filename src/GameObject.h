#pragma once



class Component;
class GameObjectManager;
class Renderable;
class MSprite;
class Renderable;


class GameObject : public std::enable_shared_from_this<GameObject>{
public:
    // Constructors
    static std::shared_ptr<GameObject> Create();
    static std::shared_ptr<GameObject> Create(const int renderLayer);
    static std::shared_ptr<GameObject> Create(const std::string& path, const sf::IntRect& rect = sf::IntRect(), int renderLayer = 0);


    ~GameObject();



    // Add a component, store as shared_ptr, return weak_ptr to the caller
    template <typename T, typename... Args>
    std::weak_ptr<T> addComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

        // check to see if component already exists. alerts you if you have multiple of the same type.
        
        //auto tryFind = getComponent<T>();
        //if (tryFind) std::cout << " Tried to add component type that already exists in component list\n";
        
         //or remove pre existing duplicate.
        //removeComponent<T>();

        // Create shared_ptr
        auto component = std::make_shared<T>(std::forward<Args>(args)...);

        // Set parent
        component->parent = weak_from_this();

        // Store in container and type map
        components.push_back(component);
        componentTypeMap[typeid(T)] = component;

        component->init();

        return component; // Return weak_ptr implicitly convertible from shared_ptr
    }

    void addComponent(std::shared_ptr<Component> component) {
        if (!component) { std::cout << "\nTried to add invalid component ptr\n"; return; }
        components.push_back(component);
    }

    // Get a component by exact type. Return shared_ptr for safe lifetime mgmt.
    template <typename T>
    std::shared_ptr<T> getComponent() {
        auto it = componentTypeMap.find(typeid(T));
        if (it != componentTypeMap.end()) {
            // Lock the weak_ptr
            auto sptr = it->second.lock();
            if (sptr) {
                return std::static_pointer_cast<T>(sptr);
            }
            else {
                // Expired, clean up stale entry
                componentTypeMap.erase(it);
            }
        }
        return nullptr;
    }

    // Get a component by base type or derived type (dynamic_cast)
    template <typename T>
    std::shared_ptr<T> getDerivativesOfComponent() {
        for (auto& comp : components) {
            auto casted = std::dynamic_pointer_cast<T>(comp);
            if (casted) {
                return casted;
            }
        }
        return nullptr;
    }

    // Remove a component by type
    template <typename T>
    bool removeComponent() {
        auto it = componentTypeMap.find(typeid(T));
        if (it != componentTypeMap.end()) {
            // Lock to find the actual shared_ptr
            auto sptr = it->second.lock();
            if (sptr) {
                // Remove from vector
                components.erase(
                    std::remove_if(components.begin(), components.end(),
                        [&](const std::shared_ptr<Component>& c) {
                            return c == sptr;
                        }),
                    components.end());
            }
            // Always erase the map entry
            componentTypeMap.erase(it);
            return true;
        }
        return false;
    }

    void removeAllComponents() {
        components.clear();
    }

    void update(float deltaTime) {

        for (size_t i = 0; i < components.size(); ) {
            components[i]->update(deltaTime);
            if (!components[i]) {
                components.erase(components.begin() + i);
            }
            else {
                i++;
            }
        }
    }


    static void Destroy(std::shared_ptr<GameObject>& gameObject);

    const std::shared_ptr<MSprite> getSprite() const {return sprite; }
    const std::shared_ptr<Renderable>& getRenderable() const { return renderable; }

    // Transform interface
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position) { setPosition(position.x, position.y); }
    void move(float offsetX, float offsetY);
    void move(const sf::Vector2f& offset) { move(offset.x, offset.y); }
    void setRotation(float angle);
    void rotate(float angle);
    void setScale(float factorX, float factorY);
    void setScale(const sf::Vector2f& factors) { setScale(factors.x, factors.y); }

    void scaleObject(float factorX, float factorY);
    void scaleObject(float factor) { scaleObject(factor, factor); }
    void scaleObject(const sf::Vector2f& factor) { scaleObject(factor.x, factor.y); }

    void setOrigin(float x, float y) { setOrigin( { x, y } ); }
    void setOrigin(const sf::Vector2f& origin);
    sf::Vector2f getOrigin();

    /// <summary>
    /// Sets the activity of a gameobject. Can set andRenderable to copy the original value to the renderables enabled state.
    /// </summary>
    void setActive(bool value, bool andRenderable = false);

    // Sprite management
    void setSprite(const std::string& path, const sf::IntRect& rect = sf::IntRect());
    void setSprite(const std::shared_ptr<sf::Texture>& texture, const sf::IntRect& rect);
    void setSprite(const sf::Texture& texture, const sf::IntRect& rect);
    void removeSprite();
    bool hasSprite() const { return sprite != nullptr; }

    void resetTransform();
    // Accessors
    const sf::Vector2f& getPosition() const { return position; }
    float getRotation() const { return rotation; }
    const sf::Vector2f& getScale() const { return scale; }
    const sf::Transform& getTransform() const;
    const bool isActive()const { return _isActive; }
    const bool ownsDrawable(const std::shared_ptr<sf::Drawable> drawable) { return sprite == drawable; }
    bool hasComponents() const { return !components.empty(); }
    void Log() const;

    void setShader(std::shared_ptr<sf::Shader> newShader);

 
    // Drawing
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    size_t getPoolIndex() { return poolIndex; }
    void setPoolIndex(size_t value) { poolIndex = value; }
private:
   
    std::vector<std::shared_ptr<Component>> components;
    std::unordered_map<std::type_index, std::weak_ptr<Component>> componentTypeMap;

   

    // Transform properties
    sf::Vector2f position{ 0.f, 0.f };
    float rotation = 0.f;
    sf::Vector2f scale{ 1.f, 1.f };
    
    bool _isActive = true;
    mutable sf::Transform transform;
    mutable bool transformNeedsUpdate = true;

    // used to significantly improve GameObjectPool finding of the object.
    size_t poolIndex = -1;
    // Rendering
    std::shared_ptr<Renderable> renderable;
    std::shared_ptr<MSprite> sprite;

    void updateTransform() const;
    GameObject();
    GameObject(const std::string& path, const sf::IntRect& rect);
};



class GameObjectManager {
public:
    // Singleton and non-copyable
    static GameObjectManager& getInstance();
    GameObjectManager(const GameObjectManager&) = delete;
    GameObjectManager& operator=(const GameObjectManager&) = delete;

    // Object management
    void add(std::weak_ptr<GameObject> obj, int renderLayer = 0);
    void addExternalRenderable(std::shared_ptr<Renderable> renderable, int layer = 0);

    void remove(std::weak_ptr<GameObject> obj);
    void removeExternalRenderable(std::shared_ptr<Renderable> renderable);
    void clearAll();

    // Update and render
    void updateAll(float deltaTime);
    void renderAll(sf::RenderTarget& target);
     

    // Layer management
    void setRenderLayer(const std::shared_ptr<GameObject>& obj, int newLayer);
    int getRenderLayer(const std::shared_ptr<GameObject>& obj) const;

    void log();

private:
    GameObjectManager() = default;
    ~GameObjectManager() = default;



    std::vector<std::weak_ptr<GameObject>> gameObjects_;
    std::map<int, std::vector<std::shared_ptr<Renderable>>> renderLayers_;
};