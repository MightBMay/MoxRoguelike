#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <memory>
struct Renderable {
	std::weak_ptr<sf::Drawable> drawable;
	std::shared_ptr<sf::Shader> shader;

	bool operator==(const Renderable& other) const {
		return drawable.lock() == other.drawable.lock();
	}
	Renderable() :shader(nullptr) {}
	Renderable(std::weak_ptr<sf::Drawable> drawable, std::shared_ptr<sf::Shader> shader = nullptr):
	drawable(drawable), shader(shader){}
};