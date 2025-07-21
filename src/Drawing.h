#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

namespace Drawing {
	void AddDrawable(int layer, std::shared_ptr<sf::Drawable> drawable);
	void RemoveDrawable(int layer, std::shared_ptr<sf::Drawable> drawable);
	void RemoveDrawable(std::shared_ptr<sf::Drawable> drawable);
	void DrawAll(sf::RenderWindow& window);
}