#pragma once
#include "Projectile.h"

class Ninja_QProjectile : public Projectile {

public:
	Ninja_QProjectile(sf::Vector2f direction, ProjectileStats stats, const sf::IntRect& textureRect) :
		Projectile(direction, stats, textureRect){}



};