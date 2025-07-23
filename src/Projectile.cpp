#include "Projectile.h"
#include "GameObject.h"
#include "EnemyMovement.h"
#include "EnemyManager.h"


Projectile::Projectile(float speed, sf::Vector2f direction, float range) :
    speed(speed), direction(direction), range(range) {}

void Projectile::init() {
    // Regular initialization (called automatically when added)
    parent->setSprite(getSpritePath());
    startPos = parent->getPosition();
}



void Projectile::update(float deltaTime) {
    parent->move(direction * speed * deltaTime);
    auto curPos = parent->getPosition();

    std::cout<< enemyManager->GetWithinRange(curPos, projectileRadius).size()<<"\n";
    if (auto playerPtr = player.lock()) { // Figure out why we aren't properly getting player ptr ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        if ((playerPtr->getPosition() - curPos).lengthSquared() < range)
            ProjectilePool::release(parent);

    }



}
void Projectile::Destroy() {}
void Projectile::ProcessEvent(const std::optional<sf::Event>& event) {}


void ProjectilePool::init(size_t initial_size) {
    // Set static references once
    Projectile::player = EnemyMovement::GetPlayer();
    Projectile::enemyManager = &EnemyManager::getInstance();

    // Initialize pool
    pool_.reserve(initial_size);
    for (size_t i = 0; i < initial_size; ++i) {
        auto obj = std::make_unique<GameObject>();
        obj->setActive(false);
        pool_.push_back(std::move(obj));
    }
}

void ProjectilePool::release(GameObject* obj) {
    obj->setActive(false);
    auto it = std::find_if(pool_.begin(), pool_.end(),
        [obj](const auto& ptr) { return ptr.get() == obj; });

    if (it != pool_.end() && it >= pool_.begin() + next_available_) {
        std::swap(*it, pool_[next_available_ - 1]);
        next_available_--;
    }
}