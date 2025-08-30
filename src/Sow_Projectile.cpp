#include "pch.h"
#include "Sow_Projectile.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"



Sow_Projectile::Sow_Projectile(sf::Vector2f direction, 	ProjectileStats stats, const sf::IntRect& textureRect):
	Projectile(direction, stats, textureRect) {

}

void Sow_Projectile::update(float deltaTime) {
	static const int minMoveDistance = 100; // snaps projectile to mouse if within this range (squared range)
	static const float _speed = *stats.speed; // avoids derefrencing projectile speed per update call.
	static const int rotationSpeed = 720;
	
	sf::Vector2f mousePos = Input::mousePos_World;
	const sf::Vector2f& curPos = parent->getPosition();
	sf::Vector2f newDirection = mousePos - curPos ;
	float distanceSqr = newDirection.lengthSquared();
	if (distanceSqr >= minMoveDistance) { // if far enough away from mouse,
		direction = newDirection.normalized();
		parent->move(direction * _speed * deltaTime);
	}
	else {// otherwise, cancel movement close enough to mouse, and set position to mouse.
		parent->setPosition(mousePos);
	}

	//parent->rotate(rotationSpeed * deltaTime); // spin projectile as some kind of animation.

	remainingDuration -= deltaTime; // decrement and handle duration.
	if (remainingDuration <= 0) {
		Projectile::projPool.release(parent);
		parent->removeComponent<TrailRenderer>(); // manually remove trailrenderer to avoid
												// it looping the last trail until new proj made.
	}

	CheckEnemies(curPos);

}

void Sow_Projectile::CheckEnemies(sf::Vector2f curPos) {
	std::vector<std::shared_ptr<GameObject>> inRangeEnemies{};
	EnemyManager::getInRange(curPos, *stats.projectileSize, inRangeEnemies);
	for (auto& enemy : inRangeEnemies) {
		if (sowedEnemies.find(enemy) == sowedEnemies.end()) {// only sow enemy if not already sowed.
			sowedEnemies.insert(enemy);
		}

		if (hitEnemies.find(enemy) != hitEnemies.end())return; // dont re _damage already damaged enemies.

		enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*stats.damage); // get the base Enemy component and take _damage.
		hitEnemies.insert(enemy); // add to hit enemies list
	}
}

void Sow_Projectile::init() {
	parent->setSprite(
		projectileAtlasTexture,
		{ {0,0},{32,32} }
	); // load the correct sprite for the projectile

	startPos = player.lock()->getPosition(); // set start position to the players position at time of making proj.
	parent->setPosition(startPos); // actually move projectile to the player position as well.
	parent->setRotation(vectorToAngle(direction));
	parent->setOrigin(16, 16);

	parent->addComponent<TrailRenderer>(0.175f, 30, sf::Color(213, 164, 106), sf::Color(184, 141, 90));// , );

}