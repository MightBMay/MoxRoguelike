#pragma once
#include "Projectile.h"

class BoomerangProjectile :public Projectile {
public:
	BoomerangProjectile(sf::Vector2f direction,
		int* damage, float* speed, float* range, int* projectileSize, int pierce) :
		Projectile(direction, damage, speed, range, projectileSize, pierce) {}

	virtual void update(float deltaTime) override;
	virtual void CheckEnemies(sf::Vector2f curPos) override;
protected:
	bool isReturning = false;
};