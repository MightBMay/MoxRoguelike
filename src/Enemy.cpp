#include "pch.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameObject.h"
#include "Player.h"



void Enemy::init() {
	LoadInfoFromJson(enemyType);
	EnemyManager::getInstance().add(parent->shared_from_this());
	hitFlickerTimer.getEndEvent().subscribe(shared_from_this(), &Enemy::ResetHitFlicker);
	// upon adding enemymovement, this gameobject becomes an enemy.
	// since GameobjectManager only uses raw pointers, and is only meant to handle drawing and updating gameobjects
	// we need a seperate manager for enemies to store their shared_ptr's.


	_maxHp  = level * hpPerLevel();
	_curHp  = _maxHp;
	_damage = level * damagePerLevel();
	_attackSpeed = attackSpeed();
	float sizeRaw = size();
	_size = sizeRaw * sizeRaw; // square size to save sqr root on distance checks.
	_speed = speed();
	_attackSize = attackSize();
	halfSize = _size / 2.0;

	setSprite();
	// have to manually add, since enemy object was created with no sprite, 
	// it isn't added to renderlayers_ due to invalid drawable ptr.
	GameObjectManager::getInstance().addExternalRenderable(parent->getRenderable(), 1);


}

void Enemy::setSprite(){
	parent->setSprite(
		"../assets/sprites/twig.png",
		sf::IntRect{ {0,0},{128,150} }
	);
	parent->setOrigin(64, 75);
	
	
}

void Enemy::Destroy() {
	auto& parentPTR = parent;
	EnemyManager::getInstance().remove(parentPTR, true);
	EnemyManager::removeHitboxVisual(parentPTR);

}

void Enemy::SetPlayer(GameObject* player) {
	_playerObj = player; // called once near playerObj creation to assign a target.
	_playerComponent = player->getDerivativesOfComponent<Player>().get();

}
void Enemy::update(float deltatime) {
	static const sf::Vector2f* playerPos = &_playerObj->getPosition();
	sf::Vector2f newDirection = (*playerPos - parent->getPosition());
	float newSqrMag = newDirection.lengthSquared();
	if (newSqrMag <= halfSize) { direction = { 0,0 }; }
	else if (newSqrMag > 0) {// avoids normalizing zero vector, which crashes.
		direction = newDirection.normalized();
		// "knockback" is handled in getspeed().
		parent->move(direction * getSpeed() * deltatime);
	}

	// checks attack timer and range check on playerObj, and deals _damage if it should. 
	if (Player::isVulnrable()) {
		Attack(deltatime, _playerObj);
	}
	hitFlickerTimer.update(deltatime);



}

void Enemy::Attack(float deltaTime, GameObject* playerObj) {
	if (attackTimer >= 0){ // if attack isn't ready, decrement timer and return;
		attackTimer -= deltaTime;
		return;
	}
	//otherwise, attack.

	// 32 is the radius of the playerObj hitbox. 
	constexpr int playerSize = 32 * 32; // squared playerObj size, because we use sqr magnitude.
	if ((playerObj->getPosition() - parent->getPosition()).lengthSquared() - playerSize <= _size) {

#pragma region make hitbox sprite
		auto hitbox = EnemyManager::getHitboxPool().make<TimedDestroy>(0,0.125f);
		if (hitbox) {
			hitbox->setSprite("../assets/sprites/shapes/circle_32.png", sf::IntRect{ {0,0},{32,32} });
			hitbox->getSprite()->setColor(sf::Color(192, 0, 0, 128));
			hitbox->setPosition(parent->getPosition());
			hitbox->setOrigin(16, 16);
			hitbox->setScale(_attackSize, _attackSize);
			GameObjectManager::getInstance().add(hitbox, 0);
		}
		else
		{
			std::cout << "\nerror making hitbox visual";
		}
#pragma endregion

		auto player = playerObj->getDerivativesOfComponent<Player>();
		player->takeDamage(_damage); // dmg playerObj (or subclass of playerObj)
		attackTimer = _attackSpeed; // reset attack timer.
	}

}

bool Enemy::takeDamage(int _damage) {
	int finalDamage = Player::getStats()->Damage(_damage);
	_curHp -= finalDamage;
	
	if (_curHp <= 0) {
		OnDeath();
		Destroy();
		return true;
	}

	hitFlickerTimer.start(true);
	parent->getSprite()->setColor(hitColour);
	return false;

}

void Enemy::OnDeath() {
	_playerComponent->AddXP(xpValue);
}


void Enemy::ResetHitFlicker() {
	parent->getSprite()->setColor(sf::Color::White);
}

void Enemy::UpdateFacingDirection() {
	if (direction.x == 0) return; // dont flip if input released.
	bool newFacingDirection = direction.x < 0; // signbit returns true if number is negative.
	if (newFacingDirection != facingDirection) { // only flip if facing direction is not the same as before.
		facingDirection = newFacingDirection; // store new facing direction
		parent->scaleObject(-1, 1); // flip playerObj.
	}
}