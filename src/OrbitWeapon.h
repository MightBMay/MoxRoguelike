#pragma once
#include "Weapon.h"
#include "OrbitProjectile.h"

class OrbitWeapon :public AutoWeapon {
public:
	OrbitWeapon():AutoWeapon(std::string("Orbit Weapon")) {
	
	
	}


	virtual void Fire() override {
		static const sf::Vector2f zero(0, 0);
		std::shared_ptr<GameObject> closestEnemy = EnemyManager::getClosest(parent.lock()->getPosition(), range);
		if (!closestEnemy) return;
		ProjectileStats stats{ &damage,&speed,&range,&projRadius,pierce };
		auto projectile = projPool.make<OrbitProjectile>(5, zero, stats, projectileRect);
		projectile->addComponent<TrailRenderer>(.125f, 16);
		attackTimer = playerStats->AttackSpeed(attackSpeed);

	}
	virtual void UpdateStats()override {

	}
	virtual void Destroy()override {}



};