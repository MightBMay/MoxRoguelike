#pragma once
#include "Weapon.h"
#include "EnemyManager.h"
class AutoWeapon : public WeaponBase {

public:

	AutoWeapon(std::string& weaponName) :WeaponBase(weaponName) {}

	virtual void Fire() override {
		auto playerPos = parent->getPosition();
		std::shared_ptr<GameObject> closestEnemy = EnemyManager::getClosest(playerPos, range);
		if (!closestEnemy) return;
		sf::Vector2f direction =  closestEnemy->getPosition() - playerPos;
		auto projectile = projPool.make<Projectile>(5, direction.normalized(), &damage,&speed,&range,&projRadius, pierce);
		attackTimer = attackSpeed;
	}
	virtual void update(float deltaTime) override{
		if (attackTimer <= 0) Fire();
		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
	}

	virtual void LevelUp()override {

	}
	virtual void Destroy()override {}


	
};