#include "GameObject.h"
#include "Component.h"



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
    for (auto& [layer, objects] : renderLayers_) {
        for (auto& obj : objects) {
            if (obj.lock()) {
                obj.lock()->draw(target, sf::RenderStates::Default);
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

void GameObjectManager::setRenderLayer(std::weak_ptr<GameObject> obj, int newLayer) {
    if (!obj.lock()) return;

    // Remove from all current layers
    for (auto& [layer, objects] : renderLayers_) {
        objects.erase(
            std::remove_if(
                objects.begin(), 
                objects.end(), 
                [&obj](const auto weak) {
                    return obj.lock() == weak.lock();
                }
            ),
            objects.end()
        );
    }

    // Add to new layer
    renderLayers_[newLayer].push_back(obj);
}

int GameObjectManager::getRenderLayer(std::weak_ptr<GameObject> obj) const {
    auto sharedObj = obj.lock();
    if (!sharedObj) return 0;

    for (const auto& [layer, objects] : renderLayers_) {
        auto it = std::find_if(objects.begin(), objects.end(),
            [&sharedObj](const std::weak_ptr<GameObject>& weak) {
                return weak.lock() == sharedObj;
            });
        if (it != objects.end()) {
            return layer;
        }
    }
    return 0;
}