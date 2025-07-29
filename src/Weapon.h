#pragma once
#include "Component.h"
#include "Utility.h"
#include "Projectile.h"
#include "MEvent.h"




struct WeaponStats
{

public:
	//s.e
	const int damage=0;
	//s.e
	const float speed=0;
	// distance the projectile travels
	const int range=0;
	// width of the collision check with enemies
	const float projRadius=0;
	// how many projectiles can be fired in a second.
	const float attackSpeed=0;
	// how many enemies the projectile can hit before being destroyed.
	const int pierce=0;


	/// <summary>
	/// remember, range and projRadius are SQUARED to avoid the root when calcualting.
	/// </summary>
	WeaponStats(int damage, float speed, int range, float projRadius, float attackSpeed, int pierce) :
		damage(damage), speed(speed), range(range* range), 
		projRadius(projRadius* projRadius), attackSpeed(1/attackSpeed),
		pierce(pierce) {}

private:

};

// need to store projectiletype somehow.
template<typename ProjectileType>
class  Weapon :public Component
{
public:

	Weapon(std::shared_ptr<WeaponStats> stats, std::shared_ptr<sf::RenderWindow> window) :
		stats(stats), window(window) {}



	virtual void Fire() {
		sf::Vector2f direction = getMouseWorldPos(*window, window->getDefaultView()) -parent->getPosition();
		projPool->make<Projectile>(direction.normalized(), stats);		
		attackTimer = stats->attackSpeed;
	}
	virtual void init() override {

		projPool = &ProjectilePool::getInstance();
	}

	virtual void update(float deltaTime)override {
		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer, stats->attackSpeed);
	}
	virtual void Destroy()override {}
	virtual void ProcessEvent(const std::optional<sf::Event>& event)override {
		if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
			switch (mousePressed->button)
			{
			case sf::Mouse::Button::Left:
				if (attackTimer <= 0) Fire(); // if fire rate's timer is done, shoot.
				break;
			case sf::Mouse::Button::Right:
			case sf::Mouse::Button::Middle:
			default:
				break;
			}
		}
	}


	std::shared_ptr<WeaponStats> getStats() const { return stats; }
	float getAttackSpeed() const { return stats->attackSpeed; }
	float getAttackTimer() const { return attackTimer; }
	MEvent<float, float>& getCooldownEvent() { return cooldownTickEvent; }


private:
	MEvent<float, float> cooldownTickEvent;

	std::shared_ptr<WeaponStats> stats;
	ProjectilePool* projPool = nullptr;
	std::shared_ptr<sf::RenderWindow> window;
	float attackTimer = 0;

};


