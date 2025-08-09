#include "OrbitProjectile.h"
#include "GameObject.h"
#include "Utility.h"
#include "Weapon.h"
#include "EnemyManager.h"
#include "Enemy.h"

void OrbitProjectile::init() {
    player_s = player.lock();
    Projectile::init();
}       // Called when component is added
void OrbitProjectile::update(float deltaTime) {



    remainingDuration -= deltaTime;
    if (remainingDuration <= 0) {
        projPool.release(parent);
        return;
    }

    // Update orbital position
    currentOrbitAngle += orbitSpeed * deltaTime;

    // Calculate new position
    sf::Vector2 offset(std::cos(currentOrbitAngle.asRadians()), std::sin(currentOrbitAngle.asRadians()));
    sf::Vector2f newPos = player_s->getPosition() + (offset * orbitRadius);
    parent->setPosition(newPos);

    // OPTIONAL: Set sprite rotation (tangent to orbit)
    sf::Vector2 tangent(-offset.y, offset.x);
    parent->setRotation(vectorToAngle(tangent));

    // update cooldown per target.
    for (auto& [enemy, cooldown] : enemyCooldowns) {
        if (cooldown > 0) {
            cooldown -= deltaTime;
        }
    }

    CheckEnemies(newPos, stats.lock());

}

void OrbitProjectile::CheckEnemies(sf::Vector2f curPos, std::shared_ptr<WeaponStats>& statsP) {
    auto inRangeEnemies = EnemyManager::getInRange(curPos, statsP->projRadius);
// iterate over all enemies within range of projectile.
    for (auto& enemy : inRangeEnemies) {
        auto it = enemyCooldowns.find(enemy); // try to find enemy in map of cooldowns
        if (it != enemyCooldowns.end() && it->second > 0) continue; // if enemy is found, and cd has not finished, continue.

        enemy->getDerivativesOfComponent<Enemy>()->takeDamage(statsP->_damage); // get the base Enemy component and take _damage.
        enemyCooldowns[enemy] = hitCooldown;
        
        --pierceCount; // decrement and check pierce.
        if (pierceCount <= 0) {
            projPool.release(parent); // if no more pierce, remove.
            // it looping the last trail until new proj made.
            break;
        }

    }
}

void OrbitProjectile::Destroy() {}