#pragma once
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Component.h"
#include "MSprite.h"





class Component;
class GameObjectManager;

class GameObject {
public:
    // Constructors
    GameObject();
    GameObject(const std::string& path, const sf::IntRect& rect = sf::IntRect());
    ~GameObject();



    //  component system 
    template <typename T, typename... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

        // First remove existing component if it exists
        removeComponent<T>();

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->parent = this;
        T* ptr = component.get();

        components.push_back(std::move(component));
        componentTypeMap[typeid(T)] = ptr;

        ptr->init();
        return ptr;
    }

    template <typename T>
    T* getComponent() {
        auto it = componentTypeMap.find(typeid(T));
        if (it != componentTypeMap.end()) {
            // Verify the component still exists in the vector
            for (auto& comp : components) {
                if (comp.get() == it->second) {
                    return static_cast<T*>(it->second);
                }
            }
            // If we get here, the pointer was stale - clean it up
            componentTypeMap.erase(it);
        }
        std::cout << " Component not found"<<std::endl;
        return nullptr;
    }

    template <typename T>
    bool removeComponent() {
        auto it = componentTypeMap.find(typeid(T));
        if (it != componentTypeMap.end()) {
            components.erase(
                std::remove_if(components.begin(), components.end(),
                    [&](auto& c) { return c.get() == it->second; }),
                components.end());
            componentTypeMap.erase(it);
            return true;
        }
        std::cout << " Component not found" << std::endl;
        return false;
    }

    void update(float deltaTime) {
        for (auto& component : components) {
            component->update(deltaTime);
        }
    }


    static void Destroy(std::shared_ptr<GameObject>& gameObject);

    const std::shared_ptr<MSprite> getSprite() const {return sprite; }

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
    void scaleObject(const sf::Vector2f& factor) { scaleObject(factor.x, factor.y); }

    void setOrigin(float x, float y) { setOrigin( { x, y } ); }
    void setOrigin(const sf::Vector2f& origin);
    void setActive(bool value) { _isActive = value; }

    // Sprite management
    void setSprite(const std::string& path, const sf::IntRect& rect = sf::IntRect());
    void removeSprite();
    bool hasSprite() const { return sprite != nullptr; }

    // Accessors
    const sf::Vector2f& getPosition() const { return position; }
    float getRotation() const { return rotation; }
    const sf::Vector2f& getScale() const { return scale; }
    const sf::Transform& getTransform() const;
    const bool isActive()const { return _isActive; }

    // Drawing
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::vector<std::unique_ptr<Component>> components;
    std::unordered_map<std::type_index, Component*> componentTypeMap;

    // Transform properties
    sf::Vector2f position{ 0.f, 0.f };
    float rotation = 0.f;
    sf::Vector2f scale{ 1.f, 1.f };
    
    bool _isActive = true;

    mutable sf::Transform transform;
    mutable bool transformNeedsUpdate = true;

    // Rendering
    std::shared_ptr<MSprite> sprite;

    void updateTransform() const;
};



class GameObjectManager {
public:
    // Singleton and non-copyable
    static GameObjectManager& getInstance();
    GameObjectManager(const GameObjectManager&) = delete;
    GameObjectManager& operator=(const GameObjectManager&) = delete;

    // Object management
    void add(GameObject* obj, int renderLayer = 0);
    void remove(GameObject* obj);
    void clearAll();

    // Update and render
    void updateAll(float deltaTime);
    void renderAll(sf::RenderTarget& target);

    // Layer management
    void setRenderLayer(GameObject* obj, int newLayer);
    int getRenderLayer(GameObject* obj) const;

private:
    GameObjectManager() = default;
    ~GameObjectManager() = default;

    std::vector<GameObject*> gameObjects_;
    std::map<int, std::vector<GameObject*>> renderLayers_;
};