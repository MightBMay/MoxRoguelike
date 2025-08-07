#pragma once
#include "Weapon.h"
#include "Sow_Projectile.h"

template<typename ProjectileType>
class Papyrmancer_Sow : public Weapon<ProjectileType> {
public:
	static inline int count = 0;
	Papyrmancer_Sow(std::shared_ptr<WeaponStats> stats, std::shared_ptr<sf::RenderWindow> window) :
		Weapon(stats, window) {}

	void Fire() override {
		static const sf::Vector2f zero = { 0,0 };
		auto projectile = Projectile::projPool.make<Sow_Projectile>(5, zero, stats);

			// spawn projectile with no direction, with ptr to weapons stats.
		attackTimer = stats->attackSpeed;

	}

	virtual void ProcessEvent(const std::optional<sf::Event>& event)override {
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			switch (keyPressed->scancode)
			{
				case sf::Keyboard::Scancode::Q:
					if (attackTimer <= 0) Fire(); // if fire rate's timer is done, shoot.
					break;
				default:
					break;
			}
		}


	}

	const std::string getDescription() const override {
		return
			"Papyrmancer Sow\nShoot a projectile that\nfollows the cursor.\nUpon hitting an enemy\napplies Sow.";
	};
};