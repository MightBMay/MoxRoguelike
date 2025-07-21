#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "MSprite.h"
#include <iostream>





class GameObject {
public:
    // Constructors
    GameObject() = default;
    GameObject(const std::string& path, const sf::IntRect& rect = sf::IntRect());
    ~GameObject() = default;

    static void Destroy(std::shared_ptr<GameObject>& gameObject) {
        if (!gameObject) return;
        gameObject->sprite->Destroy();
        gameObject.reset();
    }

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

    // Sprite management
    void setSprite(const std::string& path, const sf::IntRect& rect = sf::IntRect());
    void removeSprite();
    bool hasSprite() const { return sprite != nullptr; }

    // Accessors
    const sf::Vector2f& getPosition() const { return position; }
    float getRotation() const { return rotation; }
    const sf::Vector2f& getScale() const { return scale; }
    const sf::Transform& getTransform() const;

    // Drawing
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    // Transform properties
    sf::Vector2f position{ 0.f, 0.f };
    float rotation = 0.f;
    sf::Vector2f scale{ 1.f, 1.f };
    
    mutable sf::Transform transform;
    mutable bool transformNeedsUpdate = true;

    // Rendering
    std::shared_ptr<MSprite> sprite;

    void updateTransform() const;
};