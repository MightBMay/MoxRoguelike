#pragma once
#include "Weapon.h"
#include "Sow_Projectile.h"
#include "Animation.h"

class Papyrmancer_Sow : public AbilityBase {
public:
	static inline int count = 0;

	Papyrmancer_Sow() :	AbilityBase(std::string("Papyrmancer Sow")) {}

	void Fire() override {
		static const sf::Vector2f zero = { 0,0 };
		ProjectileStats stats{ &damage,&speed,&range,&projRadius,pierce };
		auto projectile = Projectile::projPool.make<Sow_Projectile>(5, zero, stats, projectileRect);
		
		
		auto animator = projectile->addComponent<SpriteAnimator>(*animation);
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