#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Weapon.h"


Projectile::Projectile(sf::Vector2f direction,std::weak_ptr<WeaponStats> stats) :
    stats(stats), pierceCount(stats.lock()->pierce), direction(direction){}

void Projectile::init() {
    parent->setSprite(getSpritePath()); // load the correct sprite for the projectile
    startPos = player.lock()->getPosition(); // set start position to the players position at time of making proj.
    parent->setPosition(startPos); // actually move projectile to the player position as well.
    parent->setRotation( vectorToAngle(direction) );

}



void Projectile::update(float deltaTime) {
    auto statsP = stats.lock();
    parent->move(direction * statsP->speed * deltaTime);
    auto curPos = parent->getPosition();
   
    if ((curPos - startPos).lengthSquared() >= statsP->range)
        ProjectilePool::release(parent);

    for (auto& enemy : enemyManager->GetWithinRange(curPos, statsP->projRadius)) {
        enemy->getDerivitivesOfComponent<Enemy>()->takeDamage(statsP->damage);
        --pierceCount;
        if (pierceCount <= 0) {
            ProjectilePool::release(parent);
            break;
        }
    }

}

void Projectile::Destroy() {}
void Projectile::ProcessEvent(const std::optional<sf::Event>& event) {}


void ProjectilePool::init(size_t initial_size, std::weak_ptr<GameObject> player) {
    // Set static references once
    Projectile::player = player;
    Projectile::enemyManager = &EnemyManager::getInstance();

    // Initialize pool
    pool_.reserve(initial_size);
    for (size_t i = 0; i < initial_size; ++i) {
        auto obj = std::make_unique<GameObject>();
        obj->setActive(false);
        GameObjectManager::getInstance().setRenderLayer(obj.get(), 4);
        pool_.push_back(std::move(obj));
     
    }
}

void ProjectilePool::release(GameObject* obj) {
    obj->setActive(false);
    obj->removeSprite();
    auto it = std::find_if(pool_.begin(), pool_.end(),
        [obj](const auto& ptr) { return ptr.get() == obj; });

    if (it != pool_.end() && it >= pool_.begin() + next_available_) {
        std::swap(*it, pool_[next_available_ - 1]);
        next_available_--;
    }
}