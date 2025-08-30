#pragma once
#include "Projectile.h"

class AOE_Projectile : public Projectile {

public:

	AOE_Projectile(sf::Vector2f direction, ProjectileStats stats, int aoeSize) :
		Projectile(direction, stats), aoeSize(aoeSize*aoeSize) {}
	virtual void update(float deltaTime)override;
	virtual void CheckEnemies(sf::Vector2f curPos) override;

protected:
	 float aoeSize;

	 void DoAOE(sf::Vector2f pos, float radius);

};