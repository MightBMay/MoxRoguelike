#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"
#include "TrailRenderer.h"


Projectile::Projectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats) :
	stats(stats), pierceCount(stats.lock()->pierce), direction(direction) {}

void Projectile::init() {
	parent->setSprite(getSpritePath(), { {0,0}, {32,32} }); // load the correct sprite for the projectile
	startPos = player.lock()->getPosition(); // set start position to the players position at time of making proj.
	parent->setOrigin(16, 16);
	parent->setRotation(vectorToAngle(direction));
	parent->setPosition(startPos); // actually move projectile to the player position as well.
}



void Projectile::update(float deltaTime) {
	auto statsP = stats.lock();
	parent->move(direction * statsP->speed * deltaTime);
	auto curPos = parent->getPosition();

	if ((curPos - startPos).lengthSquared() >= statsP->range) {
		projPool.release(parent);
		parent->removeComponent<TrailRenderer>(); // manually remove trailrenderer to avoid
		// it looping the last trail until new proj made.

	}

	CheckEnemies(curPos, statsP);

	

}

void Projectile::CheckEnemies(sf::Vector2f curPos, std::shared_ptr<WeaponStats>& statsP) {
	auto inRangeEnemies = EnemyManager::getInRange(curPos, statsP->projRadius);
// iterate over all enemies within range of projectile.
	for (auto& enemy : inRangeEnemies) {
		if (hitEnemies.find(enemy) != hitEnemies.end())return; // if enemy wasn't already hit by this projectile,
		enemy->getDerivativesOfComponent<Enemy>()->takeDamage(statsP->_damage); // get the base Enemy component and take _damage.
		hitEnemies.insert(enemy); // add to hit enemies list
		--pierceCount; // decrement and check pierce.
		if (pierceCount <= 0) {
			projPool.release(parent); // if no more pierce, remove.
			parent->removeComponent<TrailRenderer>(); // manually remove trailrenderer to avoid
			// it looping the last trail until new proj made.
			break;
		}

	}
}

void Projectile::Destroy() {}



