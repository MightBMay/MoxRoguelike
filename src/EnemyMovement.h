#pragma once
#include "Component.h"

class GameObject;

class EnemyMovement : public Component, public std::enable_shared_from_this<EnemyMovement> {
public:
	float speed = 80.0f;
	sf::Vector2f direction{ 0,0 };
	// 0 == right, 1 = left
	bool facingDirection = false;

	static void SetPlayer(std::weak_ptr<GameObject> player);
	static std::weak_ptr<GameObject> GetPlayer() { return _player; }

	virtual void init() override;
	void update(float deltatime) override;
	virtual void ProcessEvent(const std::optional<sf::Event>& event) {};
	~EnemyMovement();

private:
	static std::weak_ptr<GameObject> _player;
	void UpdateFacingDirection();


};