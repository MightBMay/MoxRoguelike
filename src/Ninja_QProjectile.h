#pragma once
#include "Projectile.h"

class Ninja_QProjectile : public Projectile {

public:
	Ninja_QProjectile(sf::Vector2f direction, ProjectileStats stats) :
		Projectile(direction, stats){}
protected:

};