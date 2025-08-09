#pragma once
#include "Projectile.h"

class AOE_Projectile : public Projectile {

public:

	AOE_Projectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats, float aoeSize = 256) : Projectile(direction, stats), aoeSize(aoeSize*aoeSize) {}
	virtual void update(float deltaTime)override;
	virtual void CheckEnemies(sf::Vector2f curPos, std::shared_ptr<WeaponStats>& statsP) override;

protected:
	 float aoeSize;
};