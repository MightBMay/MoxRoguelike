#include "pch.h"
#include "Player.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"

static const std::string projectileAtlasPath = "../assets/sprites/projectiles/projectileatlas.png";

Projectile::Projectile(sf::Vector2f direction, ProjectileStats stats) :
	stats(stats), direction(direction) {
	if (!projectileAtlasTexture) {
		projectileAtlasTexture = TextureManager::getTexture(projectileAtlasPath);
	}

}

void Projectile::init() {
	parent->setSprite(projectileAtlasTexture, getSpriteRect()); // load the correct sprite for the projectile
	startPos = player.lock()->getPosition(); // set start position to the players position at time of making proj.
	parent->setOrigin(16, 16);
	parent->setRotation(vectorToAngle(direction));
	parent->setPosition(startPos); // actually move projectile to the player position as well.
}



void Projectile::update(float deltaTime) {
	parent->move(direction * (*stats.speed) * deltaTime);
	auto curPos = parent->getPosition();
	auto temp = (curPos - startPos).lengthSquared();

	if ((curPos - startPos).lengthSquared() >= *stats.range) {
		projPool.release(parent);
		parent->removeComponent<TrailRenderer>(); // manually remove trailrenderer to avoid
		// it looping the last trail until new proj made.

	}

	CheckEnemies(curPos);

	

}

void Projectile::CheckEnemies(sf::Vector2f curPos) {
	std::vector<std::shared_ptr<GameObject>> inRangeEnemies{};
	EnemyManager::getInRange(curPos, *stats.projectileSize, inRangeEnemies);
// iterate over all enemies within range of projectile.
	for (auto& enemy : inRangeEnemies) {
		if (hitEnemies.find(enemy) != hitEnemies.end())return; // if enemy wasn't already hit by this projectile,
		enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*stats.damage); // get the base Enemy component and take _damage.
		hitEnemies.insert(enemy); // add to hit enemies list
		--pierceCount; // decrement and check pierce.
		if (pierceCount <= 0) {
			projPool.release(parent); // if no more pierce, remove.
			parent->removeComponent<TrailRenderer>(); // manually remove trailrenderer to avoid
			// it looping the last trail until new proj made.
			break;
		}

	}
}

void Projectile::Destroy() {}



