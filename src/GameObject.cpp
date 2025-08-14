#include "GameObject.h"
#include "Component.h"
#include "Renderable.h"
#include "Global.h"
#include "UI.h"


GameObject::GameObject() { renderable = std::make_shared<Renderable>(); };
GameObject::GameObject(const std::string& path, const sf::IntRect& rect) {
    sprite = std::make_shared<MSprite>(path, rect);
    renderable = std::make_shared<Renderable>(sprite, nullptr);
};


std::shared_ptr<GameObject> GameObject::Create(const std::string& path, const sf::IntRect& rect, int renderLayer) {
    auto obj = std::shared_ptr<GameObject>(new GameObject(path, rect));

    GameObjectManager::getInstance().add(obj, renderLayer);
    return obj;
}

std::shared_ptr<GameObject> GameObject::Create(const int renderLayer) {
    auto obj = std::shared_ptr<GameObject>(new GameObject());
    GameObjectManager::getInstance().add(obj,renderLayer);
    return obj;
}

std::shared_ptr<GameObject> GameObject::Create() {
    auto obj = std::shared_ptr<GameObject>(new GameObject());
    GameObjectManager::getInstance().add(obj);
    return obj;
}


GameObject::~GameObject() {
    components.clear();
}

// Transform methods
void GameObject::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    transformNeedsUpdate = true;
    if (sprite) sprite->setPosition({x,y});
}

void GameObject::move(float offsetX, float offsetY) {
    setPosition(position.x + offsetX, position.y + offsetY);
   
}

void GameObject::setRotation(float angle) {
    rotation = angle;
    transformNeedsUpdate = true;
    if (sprite) sprite->setRotation(sf::degrees(angle));
}

void GameObject::rotate(float angle) {
    setRotation(rotation + angle);
}

void GameObject::setScale(float factorX, float factorY) {
    scale.x = factorX;
    scale.y = factorY;
    transformNeedsUpdate = true;
    if (sprite) sprite->setScale({ factorX, factorY });
}

void GameObject::scaleObject(float factorX, float factorY) {
    setScale({scale.x * factorX, scale.y * factorY});
}

// Sprite management
void GameObject::setSprite(const std::string& path, const sf::IntRect& rect) {
    sprite = std::make_shared<MSprite>(path, rect);
    sprite->setPosition(position);
    sprite->setRotation(sf::degrees(rotation));
    sprite->setScale(scale);
    renderable->drawable = sprite;
   
 
}
void GameObject::setSprite(const sf::Texture& texture, const sf::IntRect& rect) {
    sprite = std::make_shared<MSprite>(texture, rect);
    sprite->setPosition(position);
    sprite->setRotation(sf::degrees(rotation));
    sprite->setScale(scale);
    renderable->drawable = sprite;
}


void GameObject::setOrigin(const sf::Vector2f& origin) {
    sprite->setOrigin(origin);
}


void GameObject::removeSprite() {
    if (sprite) {
        sprite->Destroy();
        renderable->drawable.reset();
    }
    sprite.reset();
}

const sf::Transform& GameObject::getTransform() const {
    if (transformNeedsUpdate) {
        updateTransform();
    }
    return transform;
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!sprite) return;
    
    target.draw(*sprite, states);
}

void GameObject::updateTransform() const {
    transform = sf::Transform::Identity;
    transform.translate(position);
    transform.rotate(sf::degrees(rotation));
    transform.scale(scale);
    transformNeedsUpdate = false;
}


void GameObject::Destroy(std::shared_ptr<GameObject>& gameObject) {
    if (!gameObject) return;
    GameObjectManager::getInstance().remove(gameObject);
    gameObject->sprite->Destroy();
    gameObject.reset();
}

void GameObject::Log() const {
    std::cout <<
        "\nisActive: "<< _isActive<<"\n"<<
        "Position: (" << position.x << ", " << position.y << ")\n" <<
        "Scale: (" << scale.x << ", " << scale.y << ")\n" <<
        "Rotation: " << rotation << "\n";

    if (hasSprite()) {
        std::cout <<
            "Sprite Path: " << sprite->getPath()<<"\n";
    }
        

}

void GameObject::setShader(std::shared_ptr<sf::Shader> newShader) { renderable->shader = newShader; }


// manager
GameObjectManager& GameObjectManager::getInstance() {
    static GameObjectManager instance;
    return instance;
}

void GameObjectManager::add(std::weak_ptr<GameObject> obj, int renderLayer) {
    auto sharedObj = obj.lock();
    if (!sharedObj) { std::cout << "gameobject ptr invalid"; return; }

    auto it = std::find_if(gameObjects_.begin(), gameObjects_.end(),
        [&sharedObj](const std::weak_ptr<GameObject>& weak) {
            return weak.lock() == sharedObj;
        });

    if (it == gameObjects_.end()) 
        gameObjects_.push_back(obj);

    
    if (sharedObj->getRenderable()->drawable.lock()) 
        renderLayers_[renderLayer].push_back(sharedObj->getRenderable()); // Default layer 0
    
   
}

void GameObjectManager::remove(std::weak_ptr<GameObject> obj) {
    auto sharedObj = obj.lock();
    const std::shared_ptr<sf::Drawable> targetDrawable = sharedObj->getRenderable()->drawable.lock();
    if (!sharedObj) return;

    // Remove from main list
    gameObjects_.erase(
        std::remove_if(gameObjects_.begin(), gameObjects_.end(),
            [&sharedObj](const std::weak_ptr<GameObject>& weak) {
                return weak.lock() == sharedObj;
            }),
        gameObjects_.end()
    );

    if (!targetDrawable) { std::cout << "drawable null\n"; return; }// exit if invalid drawable.

    for (auto& [layer, renderables] : renderLayers_) {
        // Use std::remove_if to erase matching Renderables
        auto newEnd = std::remove_if(
            renderables.begin(),
            renderables.end(),
            [&targetDrawable](const std::shared_ptr<Renderable>& r) {
                // Compare the underlying drawable pointers`
                return  r->drawable.lock() == targetDrawable;
            }
        );

        // Erase the removed elements
        renderables.erase(newEnd, renderables.end());
    }
}

void GameObjectManager::addExternalRenderable(
    std::shared_ptr<Renderable> renderable, int layer) {
    if (!renderable) { std::cout << "invalid renderable ptr"; return; }// double check ptr
    renderLayers_[layer].push_back(renderable);

}


void GameObjectManager::removeExternalRenderable(std::shared_ptr<Renderable> target) {
    for (auto& [layer, renderables] : renderLayers_) {
        auto newEnd = std::remove_if(
            renderables.begin(),
            renderables.end(),
            [&target](const std::shared_ptr<Renderable>& r) {
                return r == target && target; // && target to ensure both cannot be null.
            }

        );
        renderables.erase(newEnd, renderables.end());
    }
}

void GameObjectManager::clearAll() {
    gameObjects_.clear();
    renderLayers_.clear();
}

void GameObjectManager::updateAll(float deltaTime) {
    for (size_t i = 0; i < gameObjects_.size(); ) {
        auto obj = gameObjects_[i].lock();

        if (!obj) {
            // remove expired weak_ptrs immediately
            gameObjects_.erase(gameObjects_.begin() + i);
            continue; // skips updating and incrementing i. 
        }

        if (obj->isActive()) {
            obj->update(deltaTime); 
        }

        // only increments if element was not removed
        i++;
    }
}



void GameObjectManager::renderAll(sf::RenderTarget& target) {
    constexpr int UI_LAYER_MIN = -100;
    constexpr int UI_LAYER_MAX = 100;
    const auto defaultView = target.getDefaultView();

    bool currentViewIsUI = false;
    target.setView(*playerView); // Start with game view

    for (auto& [layer, renderables] : renderLayers_) {

        // Determine view type purely by layer number
        bool layerIsUI = (layer <= UI_LAYER_MIN) || (layer >= UI_LAYER_MAX);

        // Only switch views when crossing UI/non-UI boundary
        if (layerIsUI != currentViewIsUI) {
            target.setView(layerIsUI ? defaultView : *playerView);
            currentViewIsUI = layerIsUI;
        }

        // Draw all renderables in this layer
        for (auto& renderable : renderables) {
            if (renderable && renderable->enabled) {
              
                if (!renderable->drawable.lock()) continue;
                    

                

                if (renderable->shader) {
                    // only make state if shader exists.
                    sf::RenderStates state = sf::RenderStates::Default; 
                    state.shader = renderable->shader.get(); // get shader if it exists.

                    target.draw(*renderable->drawable.lock(), state);
                }

                // otherwise draw normally
                else target.draw(*renderable->drawable.lock());
                    
                
            }
        }
    }

    target.setView(*playerView);

}


void GameObjectManager::setRenderLayer(const std::shared_ptr<GameObject>& obj, int newLayer) {
    if (!obj) return;

    auto targetRenderable = obj->getRenderable();

    // Remove from all layers
    for (auto& [layer, renderables] : renderLayers_) {
        renderables.erase(
            std::remove_if(
                renderables.begin(),
                renderables.end(),
                [&targetRenderable](const std::shared_ptr<Renderable>& r) {
                        return r->drawable.lock() == targetRenderable->drawable.lock();
                }
            ),
            renderables.end()
        );
    }

    // Add weak reference to the authoritative Renderable
    renderLayers_[newLayer].emplace_back(targetRenderable);
}

int GameObjectManager::getRenderLayer(const std::shared_ptr<GameObject>& obj) const {
    if (!obj) return 0;



    for (const auto& [layer, renderables] : renderLayers_) {
        auto it = std::find_if(renderables.begin(), renderables.end(),
            [&obj](const std::shared_ptr<Renderable>& r) {
                return r->drawable.lock() == obj->getRenderable()->drawable.lock();
            });
        if (it != renderables.end()) {
            return layer;
        }
    }
    return 0;
}

void GameObjectManager::log() {
    std::cout << "\n#Gameobjects: " << gameObjects_.size() <<
        "\nrender layers: " << renderLayers_.size() << "\n";
    for (auto& [layer, renderables] : renderLayers_) {
        std::cout << layer << ": " << renderables.size() << "\n";
    }
}