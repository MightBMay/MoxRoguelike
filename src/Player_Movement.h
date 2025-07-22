#pragma once
#include "Component.h"

class PlayerMovement : public Component {
public:
	float speed = 50.0f;
	sf::Vector2f direction{ 0,0 };

	void update(float deltatime) override;

private:


};