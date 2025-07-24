#pragma once
#include "Component.h"

class GameObject;



class Enemy : public Component, public std::enable_shared_from_this<Enemy> {
public:

	int health = 3;


	Enemy(float speed, float size): speed(speed),size(size*size){}

	float speed = 80.0f;
	sf::Vector2f direction{ 0,0 };
	// size is SQUARED to avoid issues.
	float size = 0;

	// 0 == right, 1 = left
	bool facingDirection = false;

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