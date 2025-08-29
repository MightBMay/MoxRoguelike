#pragma once
#include "pch.h"
#include "Weapon.h"
#include "Ninja_QProjectile.h"
#include "Animation.h"

class Ninja_Q : public AbilityBase {
private:
	// stores whether its 1st, 2nd, 3rd cast
	const int maxRecasts = 3;
	int recastNum = 1;
	const float recastTimerMax = 5;
	float recastTimer = 0;
	const float spreadAngle = 15 * (PI/180);// 20 degrees as radians.
	bool recastTimedOut = true;
	SpriteAnimation animation{};
public:
	Ninja_Q():AbilityBase("Ninja Q"){
		const json& projectileData = GameData::getProjectile("Ninja Q");
		if (projectileData.contains("animation data"))		
			animation.LoadFromJson(projectileData["animation data"]);
	}
	void resetRecast() {
		
		recastNum = 1;
		attackTimer = 0.5f;// playerStats->AttackSpeed(attackSpeed);
		recastTimer = 0;
		recastTimedOut = true;
	}

	virtual void Fire() override final {
	
		sf::Vector2f mousePos = Input::mousePos_World;
		for (unsigned char i = 0; i < recastNum; ++i) {
			sf::Vector2f direction =  mousePos - parent->getPosition();
			if (direction.lengthSquared() > 0) 	direction = direction.normalized();

			// Calculate the angle offset for this projectile
			// For even number of projectiles, we want symmetric spread around the center
			float angleOffset = spreadAngle * (i - (recastNum - 1) / 2.0f);

			rotateVectorByAngle(direction, angleOffset);
			auto& proj = Projectile::projPool.make<Ninja_QProjectile>(10, direction, &damage, &speed, &range, &projRadius, pierce);
			proj->addComponent<SpriteAnimator>(animation);
		}
		recastTimedOut = false;
		recastTimer = recastTimerMax;
		++recastNum; // advance to next cast
		if (recastNum > maxRecasts) { // if final cast is cast, reset and start cooldown.
			resetRecast();
		}
		
		
	}

	virtual void update(float deltaTime) {
		if (
			Input::GetActionDown("ability1") // ability was input
			&& (attackTimer <= 0 || recastTimer > 0)) {// ability is off cooldown, or you are within the recast window.
			Fire();
		} 
			
		
		recastTimer -= deltaTime;
		if (!recastTimedOut && recastTimer <= 0)  // if you don't cast all 3 casts of the 
			resetRecast();						  // ability, it will time out and go on cd.
			
		

		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
	}


	virtual void UpdateStats() override final {}
};