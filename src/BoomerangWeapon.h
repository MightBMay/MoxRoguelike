#pragma once
#include "Weapon.h"
#include "BoomerangProjectile.h"
#include <SFML/Graphics/RenderWindow.hpp>

class BoomerangWeapon :public AutoWeapon {
public:
	BoomerangWeapon(std::shared_ptr<WeaponStats> stats) :AutoWeapon(stats) {


	}


	virtual void Fire() override {
		
		auto position = parent->getPosition();
		auto closestEnemy = EnemyManager::getClosest(position, stats->range);
		if (!closestEnemy) return; // in case no enemy was in range.
		sf::Vector2f direction = (closestEnemy->getPosition() - position);
		if(direction.lengthSquared()!=0)direction = direction.normalized();

		auto projectile = projPool.make<BoomerangProjectile>(5, direction, stats);
		attackTimer = stats->attackSpeed;

	}
	virtual void LevelUp()override {

	}
	virtual void init() override { }
	virtual void Destroy()override {}
	virtual const std::string getDescription() const override {return "Boomerang weapon"; };

};