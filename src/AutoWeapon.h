#pragma once
#include "Weapon.h"
#include "EnemyManager.h"
template<typename projectileType>
class AutoWeapon : public WeaponBase {

public:

	AutoWeapon(std::shared_ptr<WeaponStats>& stats):WeaponBase(stats){}

	virtual void Fire() override {
		std::shared_ptr<GameObject> closestEnemy = EnemyManager::getClosest(parent->getPosition(), 600);
		if (!closestEnemy) return;
		sf::Vector2f direction =  closestEnemy->getPosition() - parent->getPosition();
		auto projectile = projPool.make<projectileType>(5, direction.normalized(), stats);
		attackTimer = stats->attackSpeed;
	}
	virtual void update(float deltaTime) override{
		if (attackTimer <= 0) Fire();
		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
	}
	virtual void init() {}
	virtual void Destroy() {}
	virtual const std::string getDescription() const { return weaponDescription; }

protected:
	static inline const std::string weaponDescription = 
		"Automatically firing weapon, that targets the closest enemy within range.";
};