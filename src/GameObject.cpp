#include "GameObject.h"

GameObject::GameObject(const std::string& path, const sf::IntRect& rect): sprite(std::make_shared<MSprite>(path, rect)) {

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
    std::cout << "Moved object: (" << position.x <<", "<<position.y<<')' << std::endl;
    
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