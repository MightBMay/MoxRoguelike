#pragma once
#include "EnemyMovement.h"
#include "GameObject.h"

std::weak_ptr<GameObject> EnemyMovement::_player;

void EnemyMovement::SetPlayer(std::shared_ptr<GameObject> player) {
	_player = player;
}
void EnemyMovement::update(float deltatime) {
	if (auto player = _player.lock()) {
		sf::Vector2f playerPos = player->getPosition();
		direction = (playerPos - parent->getPosition()).normalized();
		parent->move(direction * speed * deltatime);
	}
	else {
		// player not found.
	}



}
void EnemyMovement::UpdateFacingDirection() {
	if (direction.x == 0) return; // dont flip if input released.
	bool newFacingDirection = direction.x < 0; // signbit returns true if number is negative.
	if (newFacingDirection != facingDirection) { // only flip if facing direction is not the same as before.
		facingDirection = newFacingDirection; // store new facing direction
		parent->scaleObject(-1, 1); // flip player.
	}
}