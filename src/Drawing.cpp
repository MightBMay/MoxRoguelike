#include "Drawing.h"
#include <iostream>
namespace Drawing
{
    static std::map<int, std::vector<std::shared_ptr<sf::Drawable>>> layers;

    void AddDrawable(int layer, std::shared_ptr<sf::Drawable>drawable)
    {
        layers[layer].push_back(drawable);
    }

    void RemoveDrawable(int layer, std::shared_ptr<sf::Drawable> drawable)
    {
        auto& vec = layers[layer];
        vec.erase(std::remove(vec.begin(), vec.end(), drawable), vec.end());
    }

    void RemoveDrawable(std::shared_ptr<sf::Drawable> drawable)
    {
        
        for (auto& [layer, drawables] : layers)
        {
            auto& vec = drawables;
            vec.erase(
                std::remove(vec.begin(), vec.end(), drawable),
                vec.end()
            );

        }
    }

    void DrawAll(sf::RenderWindow& window)
    {
        for (const auto& [layer, drawables] : layers)
        {
            for (const auto drawable : drawables)
            {
                if (drawable) window.draw(*drawable);
            }
        }
    }
}