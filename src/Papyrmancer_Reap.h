#pragma once
#include"Weapon.h"
#include "Sow_Projectile.h"

class Papyrmancer_Reap : public WeaponBase {
public:
	static inline int count = 0;
	Papyrmancer_Reap(std::shared_ptr<WeaponStats> stats) :
		WeaponBase(stats){}

	void Fire() override {
	
		auto& sowedEnemies = Sow_Projectile::GetSowedEnemies(); // get all enemies with Sow "debuff"
		if (sowedEnemies.size() <= 0) return;
		const sf::Vector2f zero = { 0,0 }; // can initially set direction to 0, as it is overwritten in update().
		for (auto& enemy : sowedEnemies) { // for all sowed enemies,
			if (!enemy->isActive()) continue;
			auto projectile = Projectile::projPool.make<Reap_Projectile>(5,zero, stats); 
						if (!projectile) return;
			// spawn projectile with no direction, with ptr to weapons stats.
			projectile->setPosition(enemy->getPosition()); // set position to start from the enemy.
		}
			
		Sow_Projectile::ClearSowedEnemies();
		attackTimer = stats->attackSpeed;

	}

	virtual void LevelUp()override {

	}

	virtual void update(float deltaTime)override{
		if (Input::GetActionDown("ability2")) {
			if (attackTimer <= 0) Fire();
		}
		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
	}


	const std::string getDescription() const override {
		return
			"Papyrmancer Reap\nShoot a projectile from\nall enemies marked with\nsow towards the player,\ndealing damage to enemies\non the path back.";
	};

};