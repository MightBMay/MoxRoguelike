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
	static int WeaponListSize() { return weaponList.size(); }
	static std::weak_ptr<WeaponBase> CreateWeapon(int index, std::shared_ptr<GameObject>& playerObj) {
		auto it = weaponList.find(index); // search map for index
		//if index not found, return and error log.
		if (it == weaponList.end()) { 
			std::cerr << "weapon index out of bounds / not found."; 
			return std::weak_ptr<WeaponBase>();
		}
		
		return it->second(playerObj); // calls function stored in enemylist which 
	}
	static const json& getRandomWeaponJson() {
		auto& data = *GameDataLoader::GetWeaponsData(); // get weapon data.
		static std::vector<std::string> weaponNames;


		
		//confirm data validity.
		if (!data.contains("weapons") || !data["weapons"].is_object()) { std::cerr << "\nError getting weapons data"; return json(); }
		const json& weapons = data["weapons"]; // get list of weapons.

		if (weaponNames.empty()) {// only iterate the first time.
			for (auto it = weapons.begin(); it != weapons.end(); ++it) {
				weaponNames.push_back(it.key());
			}
			std::cout << "Cached " << weaponNames.size() << " weapon names\n";
		}

		
		return GameDataLoader::getWeapon( // get weapon using name like normal.
				weaponNames[rng::getInt(0, weaponNames.size() - 1)]
				);
		
		
	
	}
	bool isMaxLevel() { return level  >= maxLevel; }
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

	
	virtual void init() { LoadInfoFromJson(); }
	virtual void Destroy() {}
	const std::string getDescription() const { return description; }


protected:
	
	static constexpr int maxLevel = 9;
	int level = 0;
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

	virtual const json& LoadInfoFromJson();
	virtual const json& GetJsonData() {  return GameDataLoader::getWeapon(name); }


	MEvent<float> cooldownTickEvent{};
	GameObjectPool<Projectile>& projPool = Projectile::projPool;
	float attackTimer = 0;
	std::shared_ptr<PlayerStats> playerStats;




};

/// <summary>
/// used for class specific abilities. not really much actual change, mainly just overrides getjsondata() to get from the abilities section.
/// </summary>
class AbilityBase : public WeaponBase {

public:
	AbilityBase(std::string abilityName):WeaponBase(abilityName){}
protected:

	//Ability base class lets me override this. this allows me to keep the index system 

	virtual const json& GetJsonData() override {  return GameDataLoader::getAbility(name); }
};









