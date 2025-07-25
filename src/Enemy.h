#pragma once
#include <iostream>
#include "Component.h"

class GameObject;



class Enemy : public Component, public std::enable_shared_from_this<Enemy> {
public:

	int health = 3;
	float speed = 0;
	int damage = 0;
	float size = 0;
	sf::Vector2f direction{ 0,0 };
	bool facingDirection; // 0== right, 1 == left;
	Enemy(int health, int damage, float speed, float size):
		health(health), damage(damage), speed(speed),size(size*size){}

	void takeDamage(int damage) {
		health -= damage;
		if (health <= 0) Destroy();
	}

	static void SetPlayer(std::weak_ptr<GameObject> player);
	static std::weak_ptr<GameObject> GetPlayer() { return _player; }

	void Destroy() override;
	
	virtual void init() override;
	void update(float deltatime) override;
	virtual void ProcessEvent(const std::optional<sf::Event>& event) {};

private:
	static std::weak_ptr<GameObject> _player;
	void UpdateFacingDirection();


};