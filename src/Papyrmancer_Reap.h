#pragma once
#include"Weapon.h"

#include "Sow_Projectile.h"
template<typename ProjectileType>
class Papyrmancer_Reap : public Weapon<ProjectileType> {
public:
	static inline int count = 0;
	Papyrmancer_Reap(std::shared_ptr<WeaponStats> stats, std::shared_ptr<sf::RenderWindow> window) :
		Weapon(stats, window) {}

	void Fire() override {
	
		auto& sowedEnemies = Sow_Projectile::GetSowedEnemies(); // get all enemies with Sow "debuff"
		if (sowedEnemies.size() <= 0) return;
		const sf::Vector2f zero = { 0,0 }; // can initially set direction to 0, as it is overwritten in update().
		for (auto& enemy : sowedEnemies) { // for all sowed enemies,
			auto projectile = Projectile::projPool.make<Reap_Projectile>(zero, stats); 
			// spawn projectile with no direction, with ptr to weapons stats.
			projectile->setPosition(enemy->getPosition()); // set position to start from the enemy.
		}
			
		Sow_Projectile::ClearSowedEnemies();
		attackTimer = stats->attackSpeed;

	}

	virtual void ProcessEvent(const std::optional<sf::Event>& event)override {
		if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
			switch (mousePressed->button)
			{
			case sf::Mouse::Button::Right:
				if (attackTimer <= 0) Fire(); // if fire rate's timer is done, shoot.
				break;
			default:
				break;
			}
		}
	}
};