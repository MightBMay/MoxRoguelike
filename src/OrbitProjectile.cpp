#include "pch.h"
#include "OrbitProjectile.h"
#include "GameObject.h"
#include "Weapon.h"
#include "EnemyManager.h"
#include "Enemy.h"

void OrbitProjectile::init() {
    player_s = player.lock();
    auto parentS = parent.lock();
    parentS->setSprite(projectileAtlasTexture, { {0,0}, {32,32} }); // load the correct sprite for the projectile
    parentS->setOrigin(16, 16);
}       // Called when component is added
void OrbitProjectile::update(float deltaTime) {


    remainingDuration -= deltaTime;
    if (remainingDuration <= 0) {
        projPool.release(parent);
        return;
    }
    auto parentS = parent.lock();
    // Update orbital position
    currentOrbitAngle += orbitSpeed * deltaTime;

    // Calculate new position
    sf::Vector2 offset(std::cos(currentOrbitAngle.asRadians()), std::sin(currentOrbitAngle.asRadians()));
    sf::Vector2f newPos = player_s->getPosition() + (offset * orbitRadius);
    parentS->setPosition(newPos);

    // OPTIONAL: Set sprite rotation (tangent to orbit)
    sf::Vector2 tangent(-offset.y, offset.x);
    parentS->setRotation(vectorToAngle(tangent));

    // update cooldown per target.
    for (auto& [enemy, cooldown] : enemyCooldowns) {
        if (cooldown > 0) {
            cooldown -= deltaTime;
        }
    }

    CheckEnemies(newPos);

}

void OrbitProjectile::CheckEnemies(sf::Vector2f curPos) {
    std::vector<std::shared_ptr<GameObject>> inRangeEnemies{};
    EnemyManager::getInRange(curPos, *stats.projectileSize, inRangeEnemies);
// iterate over all enemies within range of projectile.
    for (auto& enemy : inRangeEnemies) {
        auto it = enemyCooldowns.find(enemy); // try to find enemy in map of cooldowns
        if (it != enemyCooldowns.end() && it->second > 0) continue; // if enemy is found, and cd has not finished, continue.

        enemy->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(*stats.damage); // get the base Enemy component and take _damage.
        enemyCooldowns[enemy] = hitCooldown;
        
        --stats.pierce; // decrement and check pierce.
        if (stats.pierce <= 0) {
            projPool.release(parent); // if no more pierce, remove.
            // it looping the last trail until new proj made.
            break;
        }

    }
}

void OrbitProjectile::Destroy() {}