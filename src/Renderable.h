#pragma once
#include "pch.h"
struct Renderable {
	std::weak_ptr<sf::Drawable> drawable;
	std::weak_ptr<sf::Transformable> transform;
	std::shared_ptr<sf::Shader> shader;
	bool enabled = true;

	bool operator==(const Renderable& other) const {
		return drawable.lock() == other.drawable.lock();
	}
	Renderable() :shader(nullptr) {}
	Renderable(std::weak_ptr<sf::Drawable> drawable, std::weak_ptr<sf::Transformable> transform = {}, std::shared_ptr<sf::Shader> shader = nullptr) :
	drawable(drawable), transform(transform), shader(shader){}
};