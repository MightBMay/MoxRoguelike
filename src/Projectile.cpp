#include "pch.h"
#include "Player.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"

static const std::string projectileAtlasPath = "../assets/sprites/atlases/projectileatlas.png";

Projectile::Projectile(sf::Vector2f direction, ProjectileStats stats, const sf::IntRect& textureRect) :
	stats(stats), direction(direction), textureRect(textureRect) {
	if (!projectileAtlasTexture) {
		projectileAtlasTexture = TextureManager::getTexture(projectileAtlasPath);
	}

}

void Projectile::init() {
	auto parentS = parent.lock();
	parentS->setSprite(projectileAtlasTexture, textureRect); // load the correct sprite for the projectile
	startPos = player.lock()->getPosition(); // set start position to the players position at time of making proj.
	parentS->setOrigin(16, 16);
	parentS->setRotation(vectorToAngle(direction));
	parentS->setPosition(startPos); // actually move projectile to the player position as well.
}



void Projectile::update(float deltaTime) {
	auto parentS = parent.lock();
	parentS->move(direction * (*stats.speed) * deltaTime);
	auto curPos = parentS->getPosition();
	auto temp = (curPos - startPos).lengthSquared();

	if ((curPos - startPos).lengthSquared() >= *stats.range) {
		projPool.release(parent);
		parentS->removeComponent<TrailRenderer>(); // manually remove trailrenderer to avoid
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
			parent.lock()->removeComponent<TrailRenderer>(); // manually remove trailrenderer to avoid
			// it looping the last trail until new proj made.
			break;
		}

	}
}

void Projectile::Destroy() {}



