#pragma once
#include"Weapon.h"
#include "Sow_Projectile.h"
#include "Reap_Projectile.h"

class Papyrmancer_Reap : public AbilityBase {
public:
	static inline int count = 0;
	Papyrmancer_Reap() :
		AbilityBase(std::string("Papyrmancer Reap")) {}

	void Fire() override {
	
		auto& sowedEnemies = Sow_Projectile::GetSowedEnemies(); // get all enemies with Sow "debuff"
		if (sowedEnemies.size() <= 0) return;
		const sf::Vector2f zero = { 0,0 }; // can initially set direction to 0, as it is overwritten in update().
		ProjectileStats stats{ &damage,&speed,&range,&projRadius,pierce };
		for (auto& enemy : sowedEnemies) { // for all sowed enemies,
			if (!enemy->isActive()) continue;
			auto projectile = Projectile::projPool.make<Reap_Projectile>(5,zero, stats); 
						if (!projectile) return;
			// spawn projectile with no direction, with ptr to weapons stats.
			projectile->setPosition(enemy->getPosition()); // set position to start from the enemy.
		}
			
		Sow_Projectile::ClearSowedEnemies();
		attackTimer = playerStats->AttackSpeed(attackSpeed);

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

protected:
	const json& GetJsonData()override { return GameData::getAbility(name); }


};