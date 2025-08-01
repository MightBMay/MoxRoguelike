#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"


Projectile::Projectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats) :
	stats(stats), pierceCount(stats.lock()->pierce), direction(direction) {}

void Projectile::init() {
	parent->setSprite(getSpritePath()); // load the correct sprite for the projectile
	startPos = player.lock()->getPosition(); // set start position to the players position at time of making proj.
	parent->setPosition(startPos); // actually move projectile to the player position as well.
	parent->setRotation(vectorToAngle(direction));

}



void Projectile::update(float deltaTime) {
	auto statsP = stats.lock();
	parent->move(direction * statsP->speed * deltaTime);
	auto curPos = parent->getPosition();

	if ((curPos - startPos).lengthSquared() >= statsP->range)
		projPool.release(parent);

	auto inRangeEnemies = EnemyManager::getInstance().GetWithinRange(curPos, statsP->projRadius);
	// iterate over all enemies within range of projectile.
	for (auto& enemy : inRangeEnemies) {
		if (hitEnemies.find(enemy) != hitEnemies.end())return; // if enemy wasn't already hit by this projectile,
		enemy->getDerivativesOfComponent<Enemy>()->takeDamage(statsP->damage); // get the base Enemy component and take damage.
		hitEnemies.insert(enemy); // add to hit enemies list
		--pierceCount; // decrement and check pierce.
		if (pierceCount <= 0) {
			projPool.release(parent); // if no more pierce, remove.
			break;
		}

	}

}

void Projectile::Destroy() {}
void Projectile::ProcessEvent(const std::optional<sf::Event>& event) {}


