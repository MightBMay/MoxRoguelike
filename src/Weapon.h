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
	const int _damage=0;
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
	WeaponStats(int _damage, float speed, int range, float projRadius, float attackSpeed, int pierce) :
		_damage(_damage), speed(speed), range(range* range), 
		projRadius(projRadius* projRadius), attackSpeed(1.0f/attackSpeed),
		pierce(pierce) {}

private:

};


class WeaponBase : public Component {

private:
	// Despite class abilities being programmed as weapons, 
	// they should always be added manually in player::createweapons()

	/// <summary>
	/// List containing construction methods for weapons *NOT ABILITIES*
	/// The methods also return a pointer to the WeaponBase of the created weapon.
	/// 
	/// weapon stats are also set in the definition of the map.
	/// </summary>
	static const std::map<int, std::function<std::weak_ptr<WeaponBase>(std::shared_ptr<GameObject>)>> weaponList;



public:
	WeaponBase(std::shared_ptr<WeaponStats>& stats):stats(stats){}

	static std::weak_ptr<WeaponBase> CreateWeapon(int index, std::shared_ptr<GameObject>& playerObj) {
		auto it = weaponList.find(index); // search map for index
		//if index not found, return and error log.
		if (it == weaponList.end()) { 
			std::cerr << "weapon index out of bounds / not found."; 
			return std::weak_ptr<WeaponBase>();
		}
		
		return it->second(playerObj); // calls function stored in enemylist which 
	}

	MEvent<float>& getCooldownEvent() { return cooldownTickEvent; }

	virtual void update(float deltaTime)override {
		attackTimer -= deltaTime;
		if (attackTimer < -0.1) return;
		cooldownTickEvent.invoke(attackTimer);
	}
	virtual void Fire() = 0;
	virtual void init() = 0;
	virtual void Destroy() = 0;

	std::shared_ptr<WeaponStats> getStats() const { return stats; }
	float getAttackSpeed() const { return stats->attackSpeed; }
	float getAttackTimer() const { return attackTimer; }

	virtual const std::string getDescription() const = 0;

protected:
	std::shared_ptr<WeaponStats> stats;

	MEvent<float> cooldownTickEvent{};
	GameObjectPool<Projectile>& projPool = Projectile::projPool;
	float attackTimer = 0;


};


// need to store projectiletype somehow.
template<typename projectileType>
class  Weapon :public WeaponBase
{
public:
	Weapon(std::shared_ptr<WeaponStats> stats, std::shared_ptr<sf::RenderWindow> window) :
		WeaponBase(stats), window(window) {}



	virtual void Fire() override{
		sf::Vector2f direction = getMouseWorldPos(window, playerView) -parent->getPosition();
		auto projectile = projPool.make<projectileType>(5,direction.normalized(), stats);		
		attackTimer = stats->attackSpeed;

	}
	virtual void init() override {}
	virtual void Destroy()override {}
	virtual const std::string getDescription() const override { return "weapon test class"; };
	

protected:
	std::shared_ptr<sf::RenderWindow> window;

};









