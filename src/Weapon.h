#pragma once
#include "Component.h"
#include "Utility.h"
#include "Projectile.h"
#include "MEvent.h"
#include "Global.h"
#include "JsonLoader.h"


class PlayerStats;




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
	WeaponBase(std::string& weaponName);

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


	const int& getDamage() const { return damage; }
	const float& getSpeed() const { return speed; }
	const float& getRange() const { return range; }
	const float& getProjectileRadius() const { return projRadius; }
	const float& getPierce() const { return pierce; }
	const float& getAttackSpeed() const;
	const float& getAttackTimer() const { return attackTimer; }


	/// <summary>
	/// Must be defined in weapon subclass (projectile type must be assigned in the override).
	/// </summary>
	virtual void Fire() = 0;
	/// <summary>
	/// Handles incrementing weapon level, stats, and any special effects that occour upon leveling.
	/// </summary>
	virtual void LevelUp() = 0;

	
	virtual void init() {  }
	virtual void Destroy() {}
	const std::string getDescription() const { return description; }


protected:
	

	//s.e
	int damage=0;

	//s.e
	float speed = 0;

	// distance the projectile travels
	float range = 0;

	// width of the collision check with enemies
	int projRadius = 0;

	// how many projectiles can be fired in a second.
	float attackSpeed = 0;

	// how many enemies the projectile can hit before being destroyed.
	int pierce = 0;

	std::string description = "";
	std::string name = "";

	/// <summary>
	/// uses a string weapon name [CASE SENSITIVE] to index weapons.json and load any stats.
	/// </summary>
	virtual const json& LoadInfoFromJson(std::string weaponName);


	MEvent<float> cooldownTickEvent{};
	GameObjectPool<Projectile>& projPool = Projectile::projPool;
	float attackTimer = 0;
	std::shared_ptr<PlayerStats> playerStats;




};










