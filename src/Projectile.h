#pragma once
#include "pch.h"

class EnemyManager;
class GameObject;


constexpr size_t MAX_PROJECTILES = 512;

class Projectile : public Component {
    friend class ProjectilePool;
public:
    static inline GameObjectPool<Projectile> projPool{true};
    static inline std::weak_ptr<GameObject> player;
    int pierceCount = 0;

    sf::Vector2f direction = { 0,0 };
    // projectile stats would also go here.

 
    Projectile(sf::Vector2f direction, int* damage, float* speed, float* range, int* projectileSize, int pierce);
    virtual void init() override;       // Called when component is added
    virtual void update(float deltaTime) override;
    virtual void Destroy()override;
    virtual void CheckEnemies(sf::Vector2f curPos);
    virtual const std::string& getSpritePath() const {
        return spritePath;
    }

protected:
    
    const int* damage;
    const float* speed;
    const float* range;
    const int* projSize;


    std::unordered_set<std::shared_ptr<GameObject>> hitEnemies;
    sf::Vector2f startPos = { 0,0 };

private:
    static inline const std::string spritePath = "../assets/sprites/projectiles/default.png";

};

