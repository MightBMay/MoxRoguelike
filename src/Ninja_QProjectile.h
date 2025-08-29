#pragma once
#include "Projectile.h"

class Ninja_QProjectile : public Projectile {

public:
	Ninja_QProjectile(sf::Vector2f direction, int* damage, float* speed, float* range, int* projectileSize, int pierce) :
		Projectile(direction, damage, speed, range, projectileSize, pierce){}

protected:
	virtual const std::string& getSpritePath() const override {
		static const std::string& path = "../assets/sprites/projectiles/ninja_q.png";
		return path;
	}
};