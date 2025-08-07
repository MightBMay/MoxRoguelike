#pragma once
#include "Component.h"
#include "Utility.h"
#include <unordered_set>
#include "GameObjectPool.h"
class EnemyManager;
class GameObject;
struct WeaponStats;

constexpr size_t MAX_PROJECTILES = 512;

class Projectile : public Component {
    friend class ProjectilePool;
public:
    static inline GameObjectPool<Projectile> projPool{true};
    static inline std::weak_ptr<GameObject> player;
    int pierceCount = 0;
    sf::Vector2f direction = { 0,0 };
    // projectile stats would also go here.

    /// <summary>
    /// Creates base projectile.
    /// </summary>
    /// <param name="speed"> speed the projectile moves</param>
    /// <param name="direction"> direction of the projectile</param>
    /// <param name="range"> maximum travel distance SQUARED of the projectile.</param>
    Projectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats);
    virtual void init() override;       // Called when component is added
    virtual void update(float deltaTime) override;
    virtual void Destroy()override;
    virtual void ProcessEvent(const std::optional<sf::Event>& event)override;
    virtual const std::string& getSpritePath() const {
        return spritePath;
    }

protected:
    
    std::unordered_set<std::shared_ptr<GameObject>> hitEnemies;
    std::weak_ptr<WeaponStats> stats;
    sf::Vector2f startPos = { 0,0 };

private:
    static inline const std::string spritePath = "../assets/sprites/projectiles/default.png";

};

