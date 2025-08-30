#include "pch.h"
#include "AoeProjectile.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"


void AOE_Projectile::DoAOE(sf::Vector2f pos, float radius) {

	std::vector<std::shared_ptr<GameObject>> inAOE{};
	inAOE.reserve(16); // reserve a few, as more than 1 enemy will be hit commonly.
	EnemyManager::getInRange(pos, aoeSize, inAOE);
	for (auto& enemy : inAOE) {
		enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*stats.damage);
	}
	projPool.release(parent); // remove on first enemy since it explodes
}

void AOE_Projectile::update(float deltaTime) {
	
	parent->move(direction * (*stats.speed) * deltaTime);
	auto curPos = parent->getPosition();

	if ((curPos - startPos).lengthSquared() >= *stats.range) {
		DoAOE(curPos, aoeSize); // do aoe by default if out of range.
	}

	CheckEnemies(curPos);


}

void AOE_Projectile::CheckEnemies(sf::Vector2f curPos) {
	std::vector<std::shared_ptr<GameObject>> inRangeEnemies{};
	// cause to explode at first enemy hit. aoe == no piercing.
	auto enemy = EnemyManager::getFirstInRange(curPos, *stats.projectileSize);
	// damage actual enemy hit twice.
	enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*stats.damage);
	// iterate enemies for any within aoe radius to also damage.
	DoAOE(enemy->getPosition(), aoeSize);
}