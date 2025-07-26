#pragma once
#include <iostream>
#include "Component.h"

class GameObject;



class Enemy : public Component, public std::enable_shared_from_this<Enemy> {
public:

	int health = 3;
	float speed = 0;
	int damage = 0;
	// max number of attacks in 1 second
	float attackSpeed= 0;
	float size = 0;
	sf::Vector2f direction{ 0,0 };
	bool facingDirection =0; // 0== right, 1 == left;
	float attackTimer = 0;


	Enemy(int health, int damage, float attackSpeed, float speed, float size ):
		health(health), damage(damage), speed(speed),size(size*size), attackSpeed(1/attackSpeed){}

	void takeDamage(int damage) {
		health -= damage;
		if (health <= 0) Destroy();
	}

	virtual void DamagePlayer(float deltaTime, std::shared_ptr<GameObject>& player);

	static void SetPlayer(std::weak_ptr<GameObject> player);
	static std::weak_ptr<GameObject> GetPlayer() { return _player; }

	virtual void Destroy() override;
	
	virtual void init() override;
	virtual void update(float deltatime) override;
	virtual void ProcessEvent(const std::optional<sf::Event>& event) {};

private:
	static std::weak_ptr<GameObject> _player;
	void UpdateFacingDirection();


};