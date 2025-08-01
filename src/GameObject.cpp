#include "GameObject.h"
#include "Component.h"
#include "Global.h"
#include "UI.h"


GameObject::GameObject() { };
GameObject::GameObject(const std::string& path, const sf::IntRect& rect) {
    sprite = std::make_shared<MSprite>(path, rect);
};


std::shared_ptr<GameObject> GameObject::Create(const std::string& path, const sf::IntRect& rect) {
    auto obj = std::shared_ptr<GameObject>(new GameObject(path, rect));

    GameObjectManager::getInstance().add(obj);
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
    // Sync transform with sprite
    if (sprite) {
        sprite->setPosition(position);
        sprite->setRotation(sf::degrees(rotation));
        sprite->setScale(scale);
    }
}


void GameObject::setOrigin(const sf::Vector2f& origin) {
    sprite->setOrigin(origin);
}


void GameObject::removeSprite() {
    sprite->Destroy();
    sprite.reset();
}

const sf::Transform& GameObject::getTransform() const {
    if (transformNeedsUpdate) {
        updateTransform();
    }
    return transform;
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (sprite) {
        target.draw(*sprite, states);
    }
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


// manager
GameObjectManager& GameObjectManager::getInstance() {
    static GameObjectManager instance;
    return instance;
}

void GameObjectManager::add(std::weak_ptr<GameObject> obj, int renderLayer) {
    auto sharedObj = obj.lock();
    if (!sharedObj)return;

    auto it = std::find_if(gameObjects_.begin(), gameObjects_.end(),
        [&sharedObj](const std::weak_ptr<GameObject>& weak) {
            return weak.lock() == sharedObj;
        });

    if (it == gameObjects_.end()) {
        gameObjects_.push_back(obj);
        renderLayers_[0].push_back(obj); // Default layer 0
    }
}

void GameObjectManager::remove(std::weak_ptr<GameObject> obj) {
    auto sharedObj = obj.lock();
    if (!sharedObj) return;

    // Remove from main list
    gameObjects_.erase(
        std::remove_if(gameObjects_.begin(), gameObjects_.end(),
            [&sharedObj](const std::weak_ptr<GameObject>& weak) {
                return weak.lock() == sharedObj;
            }),
        gameObjects_.end()
    );

    // Remove from all layers
    for (auto& [layer, objects] : renderLayers_) {
        objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                [&sharedObj](const std::weak_ptr<GameObject>& weak) {
                    return weak.lock() == sharedObj;
                }),
            objects.end()
        );
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

    for (auto& [layer, objects] : renderLayers_) {
        // Determine view type purely by layer number
        bool layerIsUI = (layer <= UI_LAYER_MIN) || (layer >= UI_LAYER_MAX);

        // Only switch views when crossing UI/non-UI boundary
        if (layerIsUI != currentViewIsUI) {
            target.setView(layerIsUI ? defaultView : *playerView);
            currentViewIsUI = layerIsUI;
        }

        // Draw all objects in this layer
        for (auto& obj : objects) {
            if (auto shared = obj.lock()) {
                shared->draw(target, sf::RenderStates::Default);
            }
        }
    }

}


void GameObjectManager::processEvent(const std::optional<sf::Event>& event) {
    for (auto& objWeak : gameObjects_) {
        try {
            auto& obj = objWeak.lock();
            if (obj && obj->isActive() && obj->hasComponents()) {
                obj->handleEvent(event);
            }
        }catch (...) {
            objWeak.reset();
            std::cerr << "Tried to process event on a null gameobject pointer\n";
        }
    }
}

void GameObjectManager::setRenderLayer(const std::shared_ptr<GameObject>& obj, int newLayer) {
    if (!obj) return;

    // Remove from all current layers
    for (auto& [layer, objects] : renderLayers_) {
        objects.erase(
            std::remove_if(
                objects.begin(), 
                objects.end(), 
                [&obj](const std::weak_ptr<GameObject>& weakOther) {
                    auto other = weakOther.lock();
                    return obj == other && other;
                }
            ),
            objects.end()
        );
    }

    // Add to new layer
    renderLayers_[newLayer].push_back(obj);
}

int GameObjectManager::getRenderLayer(const std::shared_ptr<GameObject>& obj) const {
    if (!obj) return 0;

    for (const auto& [layer, weakObjects] : renderLayers_) {
        auto it = std::find_if(weakObjects.begin(), weakObjects.end(),
            [&obj](const std::weak_ptr<GameObject>& weakOther) {
                auto other = weakOther.lock();
                return other && other == obj;
            });
        if (it != weakObjects.end()) {
            return layer;
        }
    }
    return 0;
}