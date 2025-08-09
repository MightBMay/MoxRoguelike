#include "AoeProjectile.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"


void AOE_Projectile::update(float deltaTime) {
	auto statsP = stats.lock();
	parent->move(direction * statsP->speed * deltaTime);
	auto curPos = parent->getPosition();

	if ((curPos - startPos).lengthSquared() >= statsP->range) {

		// also explode at the of range.
		auto inAOE = EnemyManager::getInRange(curPos, aoeSize);
		for (auto& enemy : inAOE) {
			enemy->getDerivativesOfComponent<Enemy>()->takeDamage(statsP->_damage);
		}
		projPool.release(parent);
	}

	CheckEnemies(curPos, statsP);


}

void AOE_Projectile::CheckEnemies(sf::Vector2f curPos, std::shared_ptr<WeaponStats>& statsP) {
	auto inRangeEnemies = EnemyManager::getInRange(curPos, statsP->projRadius);
// iterate over all enemies within range of projectile.

	for (auto& enemy : inRangeEnemies) {
		// damage the enemy the projectile collides with twice.
		enemy->getDerivativesOfComponent<Enemy>()->takeDamage(statsP->_damage); 

		auto inAOE = EnemyManager::getInRange(curPos, aoeSize);
		for (auto& enemy : inAOE) {
			enemy->getDerivativesOfComponent<Enemy>()->takeDamage(statsP->_damage);
		}

		projPool.release(parent); // remove on first enemy since it explodes
		break;

	}
}