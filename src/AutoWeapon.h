#pragma once
#include "Weapon.h"
#include "EnemyManager.h"
#include "Player.h"
class AutoWeapon : public WeaponBase {

public:

	AutoWeapon(std::string& weaponName) :WeaponBase(weaponName) {}

	virtual void Fire() override {
		auto playerPos = parent->getPosition();
		std::shared_ptr<GameObject> closestEnemy = EnemyManager::getClosest(playerPos, range);
		if (!closestEnemy) return;
		sf::Vector2f direction =  closestEnemy->getPosition() - playerPos;
		if (direction.lengthSquared() > 0) direction = direction.normalized(); // avoid normalizing 0 vector.
		ProjectileStats stats{ &damage,&speed,&range,&projRadius, pierce };
		auto projectile = projPool.make<Projectile>(5, direction,stats, projectileRect);
		attackTimer = playerStats->AttackSpeed(attackSpeed);
	}
	virtual void update(float deltaTime) override{
		if (attackTimer <= 0) Fire();
		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
	}

	virtual void UpdateStats()override {
	
	}
	virtual void Destroy()override {}


	
};