#pragma once
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameObject.h"
#include "Player.h"
#include "TimedDestroy.h"


void Enemy::init() {
	EnemyManager::getInstance().add(parent->shared_from_this());
	// upon adding enemymovement, this gameobject becomes an enemy.
	// since GameobjectManager only uses raw pointers, and is only meant to handle drawing and updating gameobjects
	// we need a seperate manager for enemies to store their shared_ptr's.
}
void Enemy::Destroy() {
	auto& parentPTR = parent;
	EnemyManager::getInstance().remove( parentPTR, true);
	EnemyManager::removeHitboxVisual(parentPTR);
}

void Enemy::SetPlayer(std::weak_ptr<GameObject> player) {
	_player = player; // called once near player creation to assign a target.
}
void Enemy::update(float deltatime) { 
	if (auto player = _player.lock()) {
		sf::Vector2f playerPos = player->getPosition();
		sf::Vector2f newDirection = (playerPos - parent->getPosition());
		float newSqrMag = newDirection.lengthSquared();
		if (newSqrMag <= halfSize) { direction = { 0,0 }; }
		else if (newSqrMag > 0) {// avoids normalizing zero vector, which crashes.
			direction = newDirection.normalized();
			parent->move(direction * speed * deltatime);
		}
		
		// checks attack timer and range check on player, and deals damage if it should.
		Attack(deltatime, player);


	}
	else {
		// player not found.
	}



}

void Enemy::Attack(float deltaTime, std::shared_ptr<GameObject>& player) {
	if (attackTimer <= 0) { // if attack off cd and in range, 
		// 32 is the radius of the player hitbox. 
		constexpr int playerSize = 32 * 32; // squared player size, because we use sqr magnitude.
		if ((player->getPosition() - parent->getPosition()).lengthSquared() - playerSize <= size) {
			
#pragma region make hitbox sprite
			auto hitbox = 
			GameObject::Create("../assets/sprites/shapes/circle_32.png", sf::IntRect{ {0,0},{32,32} });

			hitbox->setPosition(parent->getPosition());
			hitbox->getSprite()->setColor(sf::Color(192, 0, 0, 128));
			hitbox->setOrigin(16, 16);
			hitbox->scaleObject(attackSize);
			hitbox->addComponent<TimedDestroy>(10);// (0.125f);
			EnemyManager::addHitboxVisual(hitbox);
#pragma endregion

			player->getDerivativesOfComponent<Player>()->takeDamage(damage); // dmg player (or subclass of player)
			attackTimer = attackSpeed; // reset attack timer.
		}
	}
	else
	{
		attackTimer -= deltaTime; // reduce attack timer.
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