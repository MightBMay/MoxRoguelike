#pragma once
#include "Weapon.h"
#include "Player.h"

class SpiralWeapon: public WeaponBase{
public:
	virtual void update(float deltaTime) override {
		if (attackTimer <= 0) {
			Fire();
			attackTimer = modifiedAttackSpeed;// modify attack speed based on how many streams there are to keep total number of projectiles the accurate.
		}
		attackTimer -= deltaTime;
									// since this expects the raw attack speed value to be max, we gotta do this.
		cooldownTickEvent.invoke(attackTimer/numberOfStreams); // otherwise the cooldown sprite subscribed here 
															// will grow too large.

		
		rotation += rotationSpeed * deltaTime;
		if (rotation > 360) rotation -= 360;
		else if (rotation < 0) rotation += 360;
	}

	virtual void Fire() override {
		for (int i = 0; i < numberOfStreams; ++i) {
			
			float centerOffset = -perStreamOffset * (numberOfStreams - 1) / 2.0f;
			float finalRotation = rotation + (perStreamOffset * i);
			sf::Vector2f direction{ 1,0 }; // start pointing straight right.
			rotateVectorByAngle(direction, finalRotation); // rotate by current rotation value.
			Projectile::projPool.make<Projectile>(5, direction, &damage, &speed, &range, &projRadius, pierce);
		}
		

	}

	virtual void UpdateStats()override {

		perStreamOffset = 2.0f*PI / numberOfStreams;
		// firing rate must slow down to keep number of projectiles per second accurate. 
		// to counteract, this when leveling up just raise attack speed by an apropriate amount.
		modifiedAttackSpeed = attackSpeed * numberOfStreams;
	}

	SpiralWeapon(std::string& weaponName): WeaponBase(weaponName){}
	virtual void init()override {
		WeaponBase::init(); 
		modifiedAttackSpeed = attackSpeed * numberOfStreams;
	}
private:
	// how many radians to rotate per second.
	static constexpr int rotationSpeed = 2;
	float rotation = 0;
	float modifiedAttackSpeed;
	int numberOfStreams = 1;
	float perStreamOffset = 2.0f* PI / numberOfStreams;
	int currentStream = 0;

};