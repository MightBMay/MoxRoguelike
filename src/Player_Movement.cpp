#include "Player_Movement.h"
#include "GameObject.h"

void PlayerMovement::update(float deltatime){
	if (direction.lengthSquared() < 0.15f) return; // only move if direction held.
	parent->move(
		direction.x * speed * deltatime,
		direction.y * speed * deltatime
	);
}