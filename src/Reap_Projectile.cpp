#include "pch.h"
#include "Reap_Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"




Reap_Projectile::Reap_Projectile(sf::Vector2f direction, ProjectileStats stats, const sf::IntRect& textureRect) :
	Projectile(direction, stats, textureRect) {

}

void Reap_Projectile::update(float deltaTime) {
	auto playerP = player.lock();
	if (!playerP) return;
	auto parentS = parent.lock();

	sf::Vector2f curPos = parentS->getPosition(); 
	
	sf::Vector2f newDirection = playerP->getPosition() - curPos;
	float distanceSqr = newDirection.lengthSquared();
	if (distanceSqr <= 25) {
		Projectile::projPool.release(parent);
		return;
	}
	direction = newDirection.normalized();
	parentS->setRotation(vectorToAngle(direction));
	parentS->move(direction * (*stats.speed)* deltaTime);

	std::vector<std::shared_ptr<GameObject>> inRangeEnemies{}; 
	EnemyManager::getInRange(curPos, *stats.projectileSize, inRangeEnemies);
	for (auto& enemy : inRangeEnemies) {
		if (hitEnemies.find(enemy) != hitEnemies.end()) return;
		enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*stats.damage); // get the base Enemy component and take _damage.
		hitEnemies.insert(enemy); // add to hit enemies list
	}

}


void Reap_Projectile::init() {
	auto parentS = parent.lock();
	parentS->setSprite(
		projectileAtlasTexture,
		{ {0,0},{32,32} }
	); // load the correct sprite for the projectile

	parentS->setRotation(vectorToAngle(direction));
	parentS->setOrigin(16, 16);
	


}