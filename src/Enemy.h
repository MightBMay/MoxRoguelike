#pragma once
#include <iostream>
#include "Component.h"
#include "Timer.h"

class GameObject;
static constexpr float hitFlickerDuration = 0.125f;
static constexpr sf::Color hitColour = sf::Color(255, 155, 155, 255);

class Enemy : public Component, public std::enable_shared_from_this<Enemy> {
public:
	
	int health = 3;
	float speed = 0;
	int damage = 0;
	// max number of attacks in 1 second
	float attackSpeed = 0;
	float size = 0;
	sf::Vector2f direction{ 0,0 };
	bool facingDirection = 0; // 0== right, 1 == left;
	float attackTimer = 0;

	// used to scale the attack hitbox visual. 
	// KEEP IN MIND this can be influenced depending on texture size for the hitbox.
	const float attackSize = 6;

	/// <summary>
	/// base enemy constructor
	/// </summary>
	/// <param name="health"></param>
	/// <param name="damage"></param>
	/// <param name="attackSpeed"> how many attacks/second the enemy can perform.</param>
	/// <param name="speed"> pixels/second movement speed</param>
	/// <param name="size"> radius of the enemies "collision"</param>
	Enemy(int health, int damage, float attackSpeed, float speed, float size) :
		health(health), damage(damage), speed(speed), size(size* size), attackSpeed(1 / attackSpeed), halfSize(size / 2) {}

	float getSpeed() {
				// can invert speed to make it simulate knockback.
		return speed * (hitFlickerTimer.inProgress() ? -0.45f : 1);
	}

	virtual bool takeDamage(int damage);

	virtual void Attack(float deltaTime, GameObject* player);

	static void SetPlayer(GameObject* player);
	static GameObject* GetPlayer() { return _player; }

	virtual void Destroy() override;

	virtual void init() override;
	virtual void update(float deltatime) override;
	virtual void ProcessEvent(const std::optional<sf::Event>& event) {};

protected:

	inline void ResetHitFlicker();

	Timer hitFlickerTimer{hitFlickerDuration, false};


private:
	static inline GameObject* _player = nullptr;
	float halfSize = 0;// used for some checks, but is a waste to calculate per frame per enemy.
	void UpdateFacingDirection();


};