#pragma once
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameObject.h"

std::weak_ptr<GameObject> Enemy::_player;


void Enemy::init() {
	EnemyManager::getInstance().add(parent->shared_from_this());
	// upon adding enemymovement, this gameobject becomes an enemy.
	// since GameobjectManager only uses raw pointers, and is only meant to handle drawing and updating gameobjects
	// we need a seperate manager for enemies to store their shared_ptr's.
}
void Enemy::Destroy() {
	EnemyManager::getInstance().remove(parent->shared_from_this(), true);
}

void Enemy::SetPlayer(std::weak_ptr<GameObject> player) {
	_player = player; // called once near player creation to assign a target.
}
void Enemy::update(float deltatime) { 
	if (auto player = _player.lock()) {
		sf::Vector2f playerPos = player->getPosition();
		sf::Vector2f newDirection = (playerPos - parent->getPosition());
		if(newDirection.lengthSquared() > 0) // avoids normalizing zero vector, which crashes.
			direction = newDirection.normalized();
		parent->move(direction * speed * deltatime);
	}
	else {
		// player not found.
	}



}
void Enemy::UpdateFacingDirection() {
	if (direction.x == 0) return; // dont flip if input released.
	bool newFacingDirection = direction.x < 0; // signbit returns true if number is negative.
	if (newFacingDirection != facingDirection) { // only flip if facing direction is not the same as before.
		facingDirection = newFacingDirection; // store new facing direction
		parent->scaleObject(-1, 1); // flip player.
	}
}