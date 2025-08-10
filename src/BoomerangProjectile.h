#pragma once
#include "Projectile.h"

class BoomerangProjectile :public Projectile {
public:
	BoomerangProjectile(sf::Vector2f direction, std::shared_ptr<WeaponStats>& stats) :
		Projectile(direction, stats) {}

	virtual void update(float deltaTime) override;
	virtual void CheckEnemies(sf::Vector2f curPos, std::shared_ptr<WeaponStats>& statsP) override;
protected:
	bool isReturning = false;
};