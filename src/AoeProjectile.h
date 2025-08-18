#pragma once
#include "Projectile.h"

class AOE_Projectile : public Projectile {

public:

	AOE_Projectile(sf::Vector2f direction, int* damage, float* speed, float* range, int* projectileSize, int pierce, int aoeSize) : 
		Projectile(direction, damage, speed, range, projectileSize, pierce), aoeSize(aoeSize*aoeSize) {}
	virtual void update(float deltaTime)override;
	virtual void CheckEnemies(sf::Vector2f curPos) override;

protected:
	 float aoeSize;
};