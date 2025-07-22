#pragma once
#include "Component.h"

class GameObject;

class EnemyMovement : public Component {
public:
	float speed = 80.0f;
	sf::Vector2f direction{ 0,0 };
	// 0 == right, 1 = left
	bool facingDirection;

	static void SetPlayer(std::shared_ptr<GameObject> player);

	void update(float deltatime) override;
	virtual void ProcessEvent(const std::optional<sf::Event>& event) {};


private:
	static std::weak_ptr<GameObject> _player;
	void UpdateFacingDirection();


};