#include "Reap_Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"
#include "Global.h"



Reap_Projectile::Reap_Projectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats) :
	Projectile(direction, stats) {

}

void Reap_Projectile::update(float deltaTime) {
	auto statsP = stats.lock();
	auto playerP = player.lock();
	if (!playerP || !statsP) return;


	sf::Vector2f curPos = parent->getPosition(); 
	
	sf::Vector2f newDirection = playerP->getPosition() - curPos;
	float distanceSqr = newDirection.lengthSquared();
	if (distanceSqr <= 25) {
		Projectile::projPool.release(parent);
		return;
	}
	direction = newDirection.normalized();
	parent->setRotation(vectorToAngle(direction));
	parent->move(direction * statsP->speed * deltaTime);


	auto inRangeEnemies = EnemyManager::getInstance().GetWithinRange(curPos, statsP->projRadius);
	for (auto& enemy : inRangeEnemies) {
		if (hitEnemies.find(enemy) != hitEnemies.end()) return;
		enemy->getDerivativesOfComponent<Enemy>()->takeDamage(statsP->damage); // get the base Enemy component and take damage.
		hitEnemies.insert(enemy); // add to hit enemies list
	}

}


void Reap_Projectile::init() {
	parent->setSprite(
		getSpritePath(),
		{ {0,0},{32,32} }
	); // load the correct sprite for the projectile

	parent->setRotation(vectorToAngle(direction));
	parent->setOrigin(16, 16);
	


}