#pragma once
#include "Projectile.h"

class BoomerangProjectile :public Projectile {
public:
	BoomerangProjectile(sf::Vector2f direction, ProjectileStats stats, const sf::IntRect& textureRect)
		: Projectile(direction, stats, textureRect) {}

	virtual void update(float deltaTime) override;
	virtual void CheckEnemies(sf::Vector2f curPos) override;
protected:
	bool isReturning = false;
};