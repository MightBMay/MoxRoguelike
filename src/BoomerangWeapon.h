#pragma once
#include "Weapon.h"
#include "BoomerangProjectile.h"
#include <SFML/Graphics/RenderWindow.hpp>


class BoomerangWeapon :public AutoWeapon {
public:
	BoomerangWeapon() :AutoWeapon(std::string("Boomerang Weapon")) {


	}


	virtual void Fire() override {
		
		auto position = parent->getPosition();
		auto closestEnemy = EnemyManager::getClosest(position, range);
		if (!closestEnemy) return; // in case no enemy was in range.
		sf::Vector2f direction = (closestEnemy->getPosition() - position);
		if(direction.lengthSquared()!=0)direction = direction.normalized();

		auto projectile = projPool.make<BoomerangProjectile>(5,direction, &damage, &speed, &range, &projRadius, pierce);
		attackTimer = playerStats->AttackSpeed(attackSpeed);

	}
	virtual void UpdateStats()override {

	}
	virtual void Destroy()override {}



};