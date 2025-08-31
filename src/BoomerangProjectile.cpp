#include "pch.h"
#include "BoomerangProjectile.h"
#include "Weapon.h"
#include "Enemy.h"
#include "EnemyManager.h"

void BoomerangProjectile::update(float deltaTime) {
	auto curPos = parent.lock()->getPosition();


	if (isReturning) {
		sf::Vector2f temp = player.lock()->getPosition()- curPos;
		
		if (temp.lengthSquared() <= 4096) {
			projPool.release(parent);
			return;
		}
		// only set direction after release would occour to skip unneeded normalize
		direction = temp.normalized();
		parent.lock()->setRotation(vectorToAngle(direction));
	}
	else if ((curPos - startPos).lengthSquared() >= *stats.range) {
		isReturning = true; // set to start returning.
		hitEnemies.clear(); // clear hit enemies so we can hit on the way back.
	}


	parent.lock()->move(direction * (*stats.speed)* deltaTime);

	CheckEnemies(curPos);


}

void BoomerangProjectile::CheckEnemies(sf::Vector2f curPos) {
	std::vector<std::shared_ptr<GameObject>> inRangeEnemies{};
	EnemyManager::getInRange(curPos, *stats.projectileSize, inRangeEnemies);
// iterate over all enemies within range of projectile.
	for (auto& enemy : inRangeEnemies) {
		if (hitEnemies.find(enemy) != hitEnemies.end())return; // if enemy wasn't already hit by this projectile,
		enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*stats.damage); // get the base Enemy component and take _damage.
		hitEnemies.insert(enemy); // add to hit enemies list
	}
}

