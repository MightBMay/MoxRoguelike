#pragma once
#include "Component.h"
#include "Utility.h"
class EnemyManager;
class GameObject;
struct WeaponStats;


class Projectile : public Component {
    friend class ProjectilePool;
public:
    int pierceCount = 0;
    sf::Vector2f direction = { 0,0 };
    static inline const std::string spritePath = "../assets/sprites/projectiles/default.png";
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

private:

    static inline std::weak_ptr<GameObject> player;
    static inline EnemyManager* enemyManager;
    std::weak_ptr<WeaponStats> stats;
    sf::Vector2f startPos = { 0,0 };



};


class ProjectilePool {
private:
    static inline std::vector<std::unique_ptr<GameObject>> pool_;
    static inline size_t next_available_ = 0;

    ProjectilePool() = default;
    ~ProjectilePool() = default;


public:

    // Deleted copy/move operations
    ProjectilePool(const ProjectilePool&) = delete;
    ProjectilePool& operator=(const ProjectilePool&) = delete;

    // Singleton access
    static ProjectilePool& getInstance() {
        static ProjectilePool instance;  // Meyer's singleton (thread-safe in C++11+)
        return instance;
    }

    // Initialize pool (call once during game setup)
    static void init(size_t initial_size, std::weak_ptr<GameObject> player);

    // Get a projectile
    template <typename ProjectileType, typename... Args>
    static GameObject* make(Args&&... args) {
        // Expand pool if exhausted
        if (next_available_ >= pool_.size()) {
            pool_.push_back(std::make_unique<GameObject>());
        }

        auto& obj = pool_[next_available_++];
        obj->setActive(true);
        obj->removeAllComponents();
        auto temp = obj->addComponent<ProjectileType>(std::forward<Args>(args)...);
        return obj.get();
    }

    // Return projectile to pool
    static void release(GameObject* obj);

    // Cleanup all projectiles
    static void clear() {
        pool_.clear();
        next_available_ = 0;
    }
};