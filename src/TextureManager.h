#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <unordered_map>
#include <string>

class TextureManager {
private:
    struct TextureManager::TextureEntry {
        std::shared_ptr<sf::Texture> texture;
        size_t referenceCount = 0;
    };
    // Static storage for the texture map
    static std::unordered_map<std::string, TextureEntry> textures;
public:
    // Get a texture (loads if not already loaded)
    static std::shared_ptr<sf::Texture> getTexture(const std::string& path);

    // Release a texture (decrements reference count, unloads if 0)
    static void releaseTexture(const std::string& path);
    static void clear() {
        textures.clear();
    }
};
