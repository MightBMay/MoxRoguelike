#pragma once
#include "Weapon.h"
#include "OrbitProjectile.h"
#include <SFML/Graphics/RenderWindow.hpp>

class OrbitWeapon :public AutoWeapon {
public:
	OrbitWeapon(std::shared_ptr<WeaponStats> stats):AutoWeapon(stats){
	
	
	}


	virtual void Fire() override {
		static const sf::Vector2f zero(0, 0);
		std::shared_ptr<GameObject> closestEnemy = EnemyManager::getClosest(parent->getPosition(), stats->range);
		if (!closestEnemy) return;
		auto projectile = projPool.make<OrbitProjectile>(5, zero, stats);
		attackTimer = stats->attackSpeed;

	}
	virtual void LevelUp()override {

	}
	virtual void init() override {}
	virtual void Destroy()override {}
	virtual const std::string getDescription() const override { return "Orbit weapon"; };

};