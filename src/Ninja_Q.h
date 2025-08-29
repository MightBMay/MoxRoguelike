#pragma once
#include "Weapon.h"

class Ninja_Q : public AbilityBase {
private:
	// stores whether its 1st, 2nd, 3rd cast
	int recastNum = 1;
	const float recastTimerMax = 8;
	float recastTimer = 10;
	static constexpr int spreadAngle = 30;
	bool recastTimedOut = false;
public:
	Ninja_Q():AbilityBase("Ninja Q"){}
	void resetRecast() {
		std::cout << "\nrecast timed out";
		recastNum = 1;
		attackTimer = playerStats->AttackSpeed(attackSpeed);
		recastTimer = 0;
		recastTimedOut = true;
	}

	virtual void Fire() override final {
		recastTimedOut = false;
		sf::Vector2f mousePos = Input::mousePos_World;
		for (int i = 0; i < recastNum; ++i) {
			sf::Vector2f direction = mousePos;
			if (direction.lengthSquared() > 0) 	direction.normalized();

			// Calculate the angle offset for this projectile
			// For even number of projectiles, we want symmetric spread around the center
			float angleOffset = spreadAngle * (i - (recastNum - 1) / 2.0f);

			rotateVectorByAngle(direction, angleOffset);

			auto& proj = Projectile::projPool.make<Projectile>(5, direction, &damage, &speed, &range, &projRadius, pierce);
			std::cout<<"\n"<<proj->getSprite()->getPath();
		}

		++recastNum;

		if (recastNum > 3) { 
			resetRecast();
		}
		
		
	}

	virtual void update(float deltaTime) {
		if (Input::GetActionDown("ability1"))// && (attackTimer <= 0 || recastTimer >0) ) 
			Fire();
		
		recastTimer -= deltaTime;
		//if (!recastTimedOut && recastTimer <= 0) 
			//resetRecast();
		

		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
	}


	virtual void UpdateStats() override final {}
};