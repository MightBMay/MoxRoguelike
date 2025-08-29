#pragma once
#include "Weapon.h"
#include "BoomerangProjectile.h"
#include "Animation.h"

class BoomerangWeapon :public AutoWeapon {

	SpriteAnimation animation{};
public:
	BoomerangWeapon() :AutoWeapon(std::string("Boomerang Weapon")) {
		const json& projectileData = GameData::getProjectile("Boomerang");
		if (projectileData.contains("animation data"))
			animation.LoadFromJson(projectileData["animation data"]);
	}


	virtual void Fire() override {
		const float projectileScaleFactor = 1.666f;
		auto position = parent->getPosition();
		auto closestEnemy = EnemyManager::getClosest(position, range);
		if (!closestEnemy) return; // in case no enemy was in range.
		sf::Vector2f direction = (closestEnemy->getPosition() - position);
		if(direction.lengthSquared()!=0)direction = direction.normalized();

		auto projectile = projPool.make<BoomerangProjectile>(5,direction, &damage, &speed, &range, &projRadius, pierce);
		projectile->addComponent<SpriteAnimator>(animation);
		projectile->scaleObject(projectileScaleFactor);
		attackTimer = playerStats->AttackSpeed(attackSpeed);

	}
	virtual void UpdateStats()override {

	}
	virtual void Destroy()override {}



};