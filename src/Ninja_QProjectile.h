#pragma once
#include "Projectile.h"

class Ninja_QProjectile : public Projectile {

public:
	Ninja_QProjectile(sf::Vector2f direction, int* damage, float* speed, float* range, int* projectileSize, int pierce) :
		Projectile(direction, damage, speed, range, projectileSize, pierce){}


};