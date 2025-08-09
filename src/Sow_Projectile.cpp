#include "Sow_Projectile.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "TrailRenderer.h"
#include "Weapon.h"
#include "Global.h"



Sow_Projectile::Sow_Projectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats):
Projectile(direction, stats){

}

void Sow_Projectile::update(float deltaTime) {
	auto statsP = stats.lock();
	static const float influenceRadiusSqr = 2500.0f; // 50px * 50px (adjust as needed)
	static const float minSpeed = statsP->speed;
	static const float maxSpeed = statsP->speed * 5.0f;
	
	sf::Vector2f mousePos = getMouseWorldPos(window, playerView);
	sf::Vector2f curPos = parent->getPosition();
	sf::Vector2f newDirection = mousePos - curPos ;
	float distanceSqr = newDirection.lengthSquared();
	if (distanceSqr >= 25) {// cancel movement if closer than 5 pixels.
		direction = newDirection.normalized();
		
		float speedFactor = std::min(distanceSqr / influenceRadiusSqr, 1.0f); // Clamped [0,1]
		float adjustedSpeed = minSpeed + (maxSpeed - minSpeed) * speedFactor;
		parent->move(direction * adjustedSpeed * deltaTime);
	}

	parent->rotate(720 * deltaTime); // spin projectile as some kind of animation.

	remainingDuration -= deltaTime;
	if (remainingDuration <= 0) {
		Projectile::projPool.release(parent);
		parent->removeComponent<TrailRenderer>(); // manually remove trailrenderer to avoid
												// it looping the last trail until new proj made.
	}
	auto inRangeEnemies = EnemyManager::getInRange(curPos, statsP->projRadius);
	for (auto& enemy : inRangeEnemies) {
		if (sowedEnemies.find(enemy) == sowedEnemies.end()) {// only sow enemy if not already sowed.
			sowedEnemies.insert(enemy);
		}

		if (hitEnemies.find(enemy) != hitEnemies.end())return; // dont re _damage already damaged enemies.
		// add to sow list wherever that is.
		enemy->getDerivativesOfComponent<Enemy>()->takeDamage(statsP->_damage); // get the base Enemy component and take _damage.
		hitEnemies.insert(enemy); // add to hit enemies list
	}

}

void Sow_Projectile::init() {
	parent->setSprite(
		getSpritePath(),
		{ {0,0},{32,32} }
	); // load the correct sprite for the projectile

	startPos = player.lock()->getPosition(); // set start position to the players position at time of making proj.
	parent->setPosition(startPos); // actually move projectile to the player position as well.
	parent->setRotation(vectorToAngle(direction));
	parent->setOrigin(16, 16);

	parent->addComponent<TrailRenderer>(0.125f, 30, sf::Color(213,164,106), sf::Color(213, 164, 106));

}