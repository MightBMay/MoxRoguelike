#pragma once
#include "Component.h"
#include "Utility.h"
#include "Projectile.h"
#include "MEvent.h"
#include "Global.h"




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
		projRadius(projRadius* projRadius), attackSpeed(1.0f/attackSpeed),
		pierce(pierce) {}

private:

};


class WeaponBase : public Component {

public:
	WeaponBase(std::shared_ptr<WeaponStats>& stats):stats(stats){}

	MEvent<float>& getCooldownEvent() { return cooldownTickEvent; }

	virtual void update(float deltaTime)override {
		attackTimer -= deltaTime;
		if (attackTimer < -0.1) return;
		cooldownTickEvent.invoke(attackTimer);
	}
	virtual void Fire() = 0;
	virtual void init() = 0;
	virtual void Destroy() = 0;
	virtual void ProcessEvent(const std::optional<sf::Event>& event) = 0;

	std::shared_ptr<WeaponStats> getStats() const { return stats; }
	float getAttackSpeed() const { return stats->attackSpeed; }
	float getAttackTimer() const { return attackTimer; }

protected:
	std::shared_ptr<WeaponStats> stats;

	MEvent<float> cooldownTickEvent{};
	GameObjectPool<Projectile>& projPool = Projectile::projPool;
	float attackTimer = 0;


};


// need to store projectiletype somehow.
template<typename ProjectileType>
class  Weapon :public WeaponBase
{
public:
	using projectileType = ProjectileType;
	Weapon(std::shared_ptr<WeaponStats> stats, std::shared_ptr<sf::RenderWindow> window) :
		WeaponBase(stats), window(window) {}



	virtual void Fire() override{
		sf::Vector2f direction = getMouseWorldPos(window, playerView) -parent->getPosition();
		projPool.make<projectileType>(direction.normalized(), stats);		
		attackTimer = stats->attackSpeed;

	}
	virtual void init() override {}
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

protected:
	std::shared_ptr<sf::RenderWindow> window;

};









