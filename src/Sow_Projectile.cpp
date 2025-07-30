#include "Sow_Projectile.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"
#include "Global.h"



Sow_Projectile::Sow_Projectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats):
Projectile(direction, stats){


}

void Sow_Projectile::update(float deltaTime) {
	auto statsP = stats.lock();
	static const float influenceRadiusSqr = 10000.0f; // 200px * 200px (adjust as needed)
	static const float minSpeed = statsP->speed;
	static const float maxSpeed = statsP->speed * 3.0f;
	
	sf::Vector2f mousePos = getMouseWorldPos(window, view);
	sf::Vector2f newDirection = mousePos - parent->getPosition();
	float distanceSqr = newDirection.lengthSquared();
	if (distanceSqr >= 25) {// cancel movement if closer than 10 pixels.
		direction = newDirection.normalized();
		
		float speedFactor = std::min(distanceSqr / influenceRadiusSqr, 1.0f); // Clamped [0,1]
		float adjustedSpeed = minSpeed + (maxSpeed - minSpeed) * speedFactor;
		parent->move(direction * adjustedSpeed * deltaTime);
	}
	parent->setRotation(vectorToAngle(direction));
}

void Sow_Projectile::init() {
	parent->setSprite(
		getSpritePath(),
		{ {0,0},{64,64} }
	); // load the correct sprite for the projectile
	startPos = player.lock()->getPosition(); // set start position to the players position at time of making proj.
	parent->setPosition(startPos); // actually move projectile to the player position as well.
	parent->setRotation(vectorToAngle(direction));
	parent->setOrigin(32, 32);

}