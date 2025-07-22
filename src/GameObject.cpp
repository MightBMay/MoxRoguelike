#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(const std::string& path, const sf::IntRect& rect)
    : GameObject() // Delegate to default constructor first
{
    sprite = std::make_shared<MSprite>(path, rect);
}

GameObject::GameObject() { GameObjectManager::getInstance().add(this); };

GameObject::~GameObject() {
    GameObjectManager::getInstance().remove(this);
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
    //sprite.reset();  does nothing currently.
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
    GameObjectManager::getInstance().remove(gameObject.get());
    gameObject->sprite->Destroy();
    gameObject.reset();
}



// manager
GameObjectManager& GameObjectManager::getInstance() {
    static GameObjectManager instance;
    return instance;
}

void GameObjectManager::add(GameObject* obj, int renderLayer) {
    if (!obj || std::find(gameObjects_.begin(), gameObjects_.end(), obj) != gameObjects_.end())
        return;
    gameObjects_.push_back(obj);
    renderLayers_[0].push_back(obj); // Default layer 0
}

void GameObjectManager::remove(GameObject* obj) {
    if (!obj) return;

    // Remove from main list
    gameObjects_.erase(
        std::remove(gameObjects_.begin(), gameObjects_.end(), obj),
        gameObjects_.end()
    );

    // Remove from all layers
    for (auto& [layer, objects] : renderLayers_) {
        objects.erase(
            std::remove(objects.begin(), objects.end(), obj),
            objects.end()
        );
    }
}

void GameObjectManager::clearAll() {
    gameObjects_.clear();
    renderLayers_.clear();
}

void GameObjectManager::updateAll(float deltaTime) {
    // Use a copy for thread safety
    auto objects = gameObjects_;
    for (auto* obj : objects) {
        if (obj && obj->isActive()) {
            obj->update(deltaTime);
        }
    }
}

void GameObjectManager::renderAll(sf::RenderTarget& target) {
    for (auto& [layer, objects] : renderLayers_) {
        for (auto* obj : objects) {
            if (obj) {
                obj->draw(target, sf::RenderStates::Default);
            }
        }
    }
}


void GameObjectManager::processEvent(const std::optional<sf::Event>& event) {
    for (auto* obj : gameObjects_) {
        if (obj && obj->isActive() && obj->hasComponents()) {
            obj->handleEvent(event);
        }
    }
}

void GameObjectManager::setRenderLayer(GameObject* obj, int newLayer) {
    if (!obj) return;

    // Remove from all current layers
    for (auto& [layer, objects] : renderLayers_) {
        objects.erase(
            std::remove(objects.begin(), objects.end(), obj),
            objects.end()
        );
    }

    // Add to new layer
    renderLayers_[newLayer].push_back(obj);
}

int GameObjectManager::getRenderLayer(GameObject* obj) const {
    if (!obj) return 0;

    for (const auto& [layer, objects] : renderLayers_) {
        if (std::find(objects.begin(), objects.end(), obj) != objects.end()) {
            return layer;
        }
    }
    return 0;
}