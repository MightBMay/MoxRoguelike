#pragma once
#include "Weapon.h"
#include "Sow_Projectile.h"
#include "Animation.h"

class Papyrmancer_Sow : public AbilityBase {
public:
	static inline int count = 0;
	static inline Animation animation{};
	Papyrmancer_Sow() :
		AbilityBase(std::string("Papyrmancer Sow")) {
		auto& projData = GameData::getProjectile("Papyrmancer Sow");
		if (projData.contains("animation data")) {
			animation.LoadFromJson(projData["animation data"]);
		}
		
	
	}

	void Fire() override {
		static const sf::Vector2f zero = { 0,0 };
		auto projectile = Projectile::projPool.make<Sow_Projectile>(5, zero, &damage,&speed,&range,&projRadius,pierce);
		
		
		auto animator = projectile->addComponent<Animator>(animation);
		animator.lock()->SetRectSize({ 48,48 });
		


		// spawn projectile with no direction, with ptr to weapons stats.
		attackTimer = playerStats->AttackSpeed(attackSpeed);

	}


	virtual void update(float deltaTime)override {
		if (Input::GetActionDown("ability1")) {
			if (attackTimer <= 0) Fire();
		}

		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
	}


};