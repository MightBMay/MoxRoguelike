#pragma once
#include "Projectile.h"
#include <unordered_map>

class Enemy;


class OrbitProjectile : public Projectile {
   
public:
    OrbitProjectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats) 
        :Projectile(direction, stats) {}
    virtual void init() override;       // Called when component is added
    virtual void update(float deltaTime) override;
    virtual void Destroy()override;

    virtual void CheckEnemies(sf::Vector2f curPos, std::shared_ptr<WeaponStats>& statsP) override;

private:
    static constexpr float orbitRadius = 150.0f; // Your desired orbit distance
    static constexpr sf::Angle orbitSpeed = sf::radians(2.0f); // Degrees per second
    static constexpr float duration = 5;
    float remainingDuration = duration;

    static inline std::shared_ptr<GameObject> player_s = nullptr;

    sf::Angle currentOrbitAngle = sf::Angle::Zero;

  
    std::unordered_map<std::shared_ptr<GameObject>, float > enemyCooldowns;
    static constexpr float hitCooldown = 0.33f;

   
    
    
    


};