#include "BoomerangProjectile.h"
#include "Weapon.h"
#include "Utility.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include <SFML/System/Vector2.hpp>
void BoomerangProjectile::update(float deltaTime) {
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
	else if ((curPos - startPos).lengthSquared() >= *range) {
		isReturning = true; // set to start returning.
		hitEnemies.clear(); // clear hit enemies so we can hit on the way back.
	}


	parent->move(direction * (*speed)* deltaTime);

	CheckEnemies(curPos);


}

void BoomerangProjectile::CheckEnemies(sf::Vector2f curPos) {
	auto inRangeEnemies = EnemyManager::getInRange(curPos, *projSize);
// iterate over all enemies within range of projectile.
	for (auto& enemy : inRangeEnemies) {
		if (hitEnemies.find(enemy) != hitEnemies.end())return; // if enemy wasn't already hit by this projectile,
		enemy->getDerivativesOfComponent<Enemy>()->takeDamage(*damage); // get the base Enemy component and take _damage.
		hitEnemies.insert(enemy); // add to hit enemies list
	}
}

