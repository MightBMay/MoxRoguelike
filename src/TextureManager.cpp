#include "pch.h"

// Define the TextureEntry struct (private implementation)

// Define the static member
std::unordered_map<std::string, TextureManager::TextureEntry> TextureManager::textures;




std::shared_ptr<sf::Texture> TextureManager::getTexture(const std::string& path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        it->second.referenceCount++;
        return it->second.texture;
    }

    // Load new texture
    auto newTexture = std::make_shared<sf::Texture>();
    if (!newTexture->loadFromFile(path)) {
        throw std::runtime_error("Failed to load texture: " + path);
    }

    textures[path] = { newTexture, 1 };
    return newTexture;
}

void TextureManager::releaseTexture(const std::string& path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        it->second.referenceCount--;
        if (it->second.referenceCount == 0) {
            textures.erase(it);
        }
    }
}