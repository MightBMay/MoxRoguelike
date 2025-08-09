#pragma once
#include "Weapon.h"
#include <SFML/Graphics/RenderWindow.hpp>
template<typename projectileType>
class OrbitWeapon :public WeaponBase {
	static_assert(std::is_base_of_v<OrbitProjectile, projectileType>,
		"projectileType must be derived from OrbitProjectile");
public:
	OrbitWeapon(std::shared_ptr<WeaponStats>, std::shared_ptr<sf::RenderWindow> window):WeaponBase(stats, window){
	
	
	}


	virtual void Fire() override {
		static const sf::Vector2f zero(0, 0);
		auto projectile = projPool.make<projectileType>(5, zero, stats);
		attackTimer = stats->attackSpeed;

	}
	virtual void init() override {}
	virtual void Destroy()override {}
	virtual const std::string getDescription() const override { return "weapon test class"; };

};