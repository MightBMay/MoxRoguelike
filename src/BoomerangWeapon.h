#pragma once
#include "Weapon.h"
#include "AutoWeapon.h"
#include "BoomerangProjectile.h"
#include "Animation.h"

class BoomerangWeapon :public AutoWeapon {
public:
	BoomerangWeapon() :AutoWeapon(std::string("Boomerang Weapon")) {}


	virtual void Fire() override {
		const float projectileScaleFactor = 1.666f;
		auto position = parent.lock()->getPosition();
		auto closestEnemy = EnemyManager::getClosest(position, range);
		if (!closestEnemy) return; // in case no enemy was in range.
		sf::Vector2f direction = (closestEnemy->getPosition() - position);
		if(direction.lengthSquared()!=0) direction = direction.normalized();
		ProjectileStats stats{ &damage, &speed, &range, &projRadius, pierce };
		auto projectile = projPool.make<BoomerangProjectile>(5,direction, stats, projectileRect);
		projectile->addComponent<SpriteAnimator>(*animation);
		projectile->scaleObject(projectileScaleFactor);
		attackTimer = playerStats->AttackSpeed(attackSpeed);

	}
	virtual void UpdateStats()override {

	}
	virtual void Destroy()override {}



};