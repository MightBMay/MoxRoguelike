#pragma once
#include "Component.h"

class PlayerMovement : public Component {
public:
	float speed = 110.0f;
	sf::Vector2f direction{ 0,0 };
	// 0 == right, 1 = left
	bool facingDirection;
	void update(float deltatime) override;
	void ProcessEvent(const std::optional<sf::Event>& event)override;

private:

	void UpdateFacingDirection();


};