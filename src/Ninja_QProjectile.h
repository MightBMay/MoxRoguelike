#pragma once
#include "Projectile.h"

class Ninja_QProjectile : public Projectile {

public:
	Ninja_QProjectile(sf::Vector2f direction, ProjectileStats stats, const sf::IntRect& textureRect) :
		Projectile(direction, stats, textureRect){}

	virtual void update(float deltatime) override {
		Projectile::update(deltatime);
		std::cerr << "\nSpeed: " << *stats.speed;
	}
protected:

};