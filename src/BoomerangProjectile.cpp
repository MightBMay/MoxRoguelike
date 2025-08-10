#include "BoomerangProjectile.h"
#include "Weapon.h"
#include "Utility.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include <SFML/System/Vector2.hpp>
void BoomerangProjectile::update(float deltaTime) {
	auto statsP = stats.lock();
	auto curPos = parent->getPosition();


	if (isReturning) {
		sf::Vector2f temp = player.lock()->getPosition()- curPos;
		
		if (temp.lengthSquared() <= 4096) {
			projPool.release(parent);
			return;
		}
		// only set direction after release would occour to skip unneeded normalize
		direction = temp.normalized();
		parent->setRotation(vectorToAngle(direction));
	}
	else if ((curPos - startPos).lengthSquared() >= statsP->range) {
		isReturning = true; // set to start returning.
		hitEnemies.clear(); // clear hit enemies so we can hit on the way back.
	}


	parent->move(direction * statsP->speed * deltaTime);

	CheckEnemies(curPos, statsP);


}

void BoomerangProjectile::CheckEnemies(sf::Vector2f curPos, std::shared_ptr<WeaponStats>& statsP) {
	auto inRangeEnemies = EnemyManager::getInRange(curPos, statsP->projRadius);
// iterate over all enemies within range of projectile.
	for (auto& enemy : inRangeEnemies) {
		if (hitEnemies.find(enemy) != hitEnemies.end())return; // if enemy wasn't already hit by this projectile,
		enemy->getDerivativesOfComponent<Enemy>()->takeDamage(statsP->_damage); // get the base Enemy component and take _damage.
		hitEnemies.insert(enemy); // add to hit enemies list
	}
}

