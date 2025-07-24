#pragma once
#include "Component.h"
#include "Utility.h"
#include "Projectile.h"




struct WeaponStats
{

public:
	//s.e
	const int damage;
	//s.e
	const float speed;
	// distance the projectile travels
	const int range;
	// width of the collision check with enemies
	const float projRadius;
	// how many projectiles can be fired in a second.
	const float fireRate;


	/// <summary>
	/// remember, range and projRadius are SQUARED to avoid the root when calcualting.
	/// </summary>
	WeaponStats(int damage, float speed, int range, float projRadius, float fireRate) :
		damage(damage), speed(speed), range(range* range), projRadius(projRadius* projRadius), fireRate(fireRate) {}

private:

};


template<typename ProjectileType>
class  Weapon :public Component
{
public:

	Weapon(std::shared_ptr<WeaponStats> stats, std::shared_ptr<sf::RenderWindow> window) :
		stats(stats), window(window) {}

	virtual void Fire() {
		sf::Vector2f direction = getMouseWorldPos(*window, window->getDefaultView()) -parent->getPosition();
		projPool->make<Projectile>(stats->speed, direction.normalized(), stats->range, stats->projRadius);
		fireTimer = 1 / stats->fireRate;
	}
	virtual void init() override {

		projPool = &ProjectilePool::getInstance();
	}

	virtual void update(float deltaTime)override { fireTimer -= deltaTime; }
	virtual void Destroy()override {}
	virtual void ProcessEvent(const std::optional<sf::Event>& event)override {
		if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
			switch (mousePressed->button)
			{
			case sf::Mouse::Button::Left:
				if (fireTimer <= 0) Fire(); // if fire rate's timer is done, shoot.
				break;
			case sf::Mouse::Button::Right:
			case sf::Mouse::Button::Middle:
			default:
				break;
			}
		}
	}

private:
	using projectileT = ProjectileType;
	std::shared_ptr<WeaponStats> stats;
	ProjectilePool* projPool = nullptr;
	std::shared_ptr<sf::RenderWindow> window;
	float fireTimer = 0;

};


