#include "pch.h"
#include "AoeProjectile.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"


void AOE_Projectile::update(float deltaTime) {
	
	parent->move(direction * (*speed) * deltaTime);
	auto curPos = parent->getPosition();

	if ((curPos - startPos).lengthSquared() >= *range) {

		// also explode at the of range.
		auto inAOE = EnemyManager::getInRange(curPos, aoeSize);
		for (auto& enemy : inAOE) {
			enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*damage);
		}
		projPool.release(parent);
	}

	CheckEnemies(curPos);


}

void AOE_Projectile::CheckEnemies(sf::Vector2f curPos) {
	auto inRangeEnemies = EnemyManager::getInRange(curPos, *projSize);
// iterate over all enemies within range of projectile.

	for (auto& enemy : inRangeEnemies) {
		// damage the enemy the projectile collides with twice.
		enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*damage);

		auto inAOE = EnemyManager::getInRange(curPos, aoeSize);
		for (auto& enemy : inAOE) {
			enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*damage);
		}

		projPool.release(parent); // remove on first enemy since it explodes
		break;

	}
}