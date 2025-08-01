#pragma once
#include <SFML/Graphics.hpp>
#include "Drawing.h"
#include <memory>
#include <string>
#include "TextureManager.h"

/// <summary>
/// Custom sprite class to cache loaded textures, avoiding duplicates in memory.
/// </summary>
class MSprite : public sf::Sprite, public std::enable_shared_from_this<MSprite> {
private:
    std::string texturePath;
    std::shared_ptr<sf::Texture> textureRef;

    

public:
    MSprite(const std::string& path, const sf::IntRect& rect = sf::IntRect())
        : sf::Sprite(*LoadTexture(path)),  // base needs texture immediately
        textureRef(TextureManager::getTexture(path)),  // keep ref alive
        texturePath(path)
    {
        if (rect != sf::IntRect()) {
            setTextureRect(rect);
        }
    }


    MSprite(std::shared_ptr<sf::Texture> texture) : sf::Sprite(*texture), textureRef(texture) {}
    ~MSprite() {
    }


    void SetRepeated(bool value) {
        textureRef->setRepeated(value);
    }
    

    static std::shared_ptr<sf::Texture> LoadTexture(const std::string& path) {
        return TextureManager::getTexture(path);
    }
    std::string getPath() const { return texturePath; }
    const void Destroy() {
        TextureManager::releaseTexture(texturePath);
    
    }

    // Disable copying
    MSprite(const MSprite&) = delete;
    MSprite& operator=(const MSprite&) = delete;

    // Enable moving
    MSprite(MSprite&& other) noexcept
        : sf::Sprite(std::move(other)),
        textureRef(std::move(other.textureRef)),
        texturePath(std::move(other.texturePath)) {
        other.textureRef = nullptr;
    }

    MSprite& operator=(MSprite&& other) noexcept {
        if (this != &other) {
            sf::Sprite::operator=(std::move(other));
            textureRef = std::move(other.textureRef);
            texturePath = std::move(other.texturePath);
            other.textureRef = nullptr;
        }
        return *this;
    }
};