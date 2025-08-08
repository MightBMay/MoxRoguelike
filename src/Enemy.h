#pragma once
#include <iostream>
#include "Component.h"
#include "Timer.h"

class GameObject;

class Enemy : public Component, public std::enable_shared_from_this<Enemy> {
public:
	
	int level = 1;

	int _maxHp=0;
	int _curHp=0;
	int _damage=0;
	float _attackSpeed=0;

	float _speed=0;
	float _size=0;
	float _attackSize=0;
	
	// max number of attacks in 1 second
	float attackTimer = 0;
	
	sf::Vector2f direction{ 0,0 };

	bool facingDirection = 0; // 0== right, 1 == left;




	/// <summary>
	/// base enemy constructor
	/// </summary>
	Enemy(int level) :level(level){}  // having pure virtual methods called like this FORCES all enemy subclasses to define their stats.
		 

	float getSpeed() {
				// can invert speed to make it simulate knockback.
		return _speed * (hitFlickerTimer.inProgress() ? -0.45f : 1);
	}

	virtual bool takeDamage(int _damage);

	virtual void Attack(float deltaTime, GameObject* player);

	static void SetPlayer(GameObject* player);
	static GameObject* GetPlayer() { return _player; }
	virtual void setSprite();
	virtual void log() {
		std::cout <<
			"\nEnemy: " <<
			"\nlevel: " << level <<
			"\nhp: " << _curHp << "/ " << _maxHp <<
			"\ndamage: " << _damage <<
			"\nspeed: " << _speed<<
			"\nsize: " << _size;

	}

	virtual void Destroy() override;

	virtual void init() override;
	virtual void update(float deltatime) override;


protected:

	inline void ResetHitFlicker();

	Timer hitFlickerTimer{hitFlickerDuration, false};

	// amount of hp gained per level.
	virtual int hpPerLevel() const = 0;
	// amount of _damage gained per level
	virtual int damagePerLevel() const =0;
	/// <summary>
	/// attack speed (returns number of attacks per second).
	/// </summary>
	virtual float attackSpeed() const = 0;

	/// <summary>
	/// used to scale the attack hitbox visual. KEEP IN MIND this can be influenced depending on texture size for the hitbox.
	/// </summary>
	virtual const float attackSize() const = 0;
	virtual float speed() const = 0;
	virtual float size() const = 0;



private:
	static constexpr float hitFlickerDuration = 0.125f;
	static constexpr sf::Color hitColour = sf::Color(255, 155, 155, 255);


	static inline GameObject* _player = nullptr;
	float halfSize;// used for some checks, but is a waste to calculate per frame per enemy.
	void UpdateFacingDirection();


};