#pragma once
#include "Weapon.h"
#include "Sow_Projectile.h"


class Papyrmancer_Sow : public WeaponBase {
public:
	static inline int count = 0;
	Papyrmancer_Sow() :
		WeaponBase(std::string("Papyrmancer Sow")) {}

	void Fire() override {
		static const sf::Vector2f zero = { 0,0 };
		auto projectile = Projectile::projPool.make<Sow_Projectile>(5, zero, &damage,&speed,&range,&projRadius,pierce);

			// spawn projectile with no direction, with ptr to weapons stats.
		attackTimer = attackSpeed;

	}

	virtual void LevelUp()override {

	}

	virtual void update(float deltaTime)override {
		if (Input::GetActionDown("ability1")) {
			if (attackTimer <= 0) Fire();
		}

		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
	}

	const std::string getDescription() const override {
		return
			"Papyrmancer Sow\nShoot a projectile that\nfollows the cursor.\nUpon hitting an enemy\napplies Sow.";
	};


};