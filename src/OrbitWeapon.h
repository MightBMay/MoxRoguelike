#pragma once
#include "Weapon.h"
#include "OrbitProjectile.h"
#include <SFML/Graphics/RenderWindow.hpp>

class OrbitWeapon :public AutoWeapon {
public:
	OrbitWeapon():AutoWeapon(std::string("Orbit Weapon")) {
	
	
	}


	virtual void Fire() override {
		static const sf::Vector2f zero(0, 0);
		std::shared_ptr<GameObject> closestEnemy = EnemyManager::getClosest(parent->getPosition(), range);
		if (!closestEnemy) return;
		auto projectile = projPool.make<OrbitProjectile>(5, zero, &damage,&speed,&range,&projRadius,pierce);
		attackTimer = playerStats->AttackSpeed(attackSpeed);

	}
	virtual void LevelUp()override {

	}
	virtual void Destroy()override {}



};