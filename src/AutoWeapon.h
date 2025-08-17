#pragma once
#include "Weapon.h"
#include "EnemyManager.h"
class AutoWeapon : public WeaponBase {

public:

	AutoWeapon(std::shared_ptr<WeaponStats>& stats):WeaponBase(stats){}

	virtual void Fire() override {
		auto playerPos = parent->getPosition();
		std::shared_ptr<GameObject> closestEnemy = EnemyManager::getClosest(playerPos, stats->range);
		if (!closestEnemy) return;
		sf::Vector2f direction =  closestEnemy->getPosition() - playerPos;
		auto projectile = projPool.make<Projectile>(5, direction.normalized(), stats);
		attackTimer = stats->attackSpeed;
	}
	virtual void update(float deltaTime) override{
		if (attackTimer <= 0) Fire();
		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
	}

	virtual void LevelUp()override {

	}

	virtual void init()override { }
	virtual void Destroy()override {}
	virtual const std::string getDescription() const override {return weaponDescription; }

protected:
	std::string weaponDescription =
		"Automatically firing weapon,\n that targets the closest enemy within range.";
	
};