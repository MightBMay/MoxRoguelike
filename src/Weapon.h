#pragma once
#include "Projectile.h"
class SpriteAnimation;
class Component;
class PlayerStats;



class WeaponBase : public Component {

private:

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
		auto& data = *GameData::GetWeaponsData(); // get weapon data.
		static std::vector<std::string> weaponNames;


		
		//confirm data validity.
		if (!data.contains("weapons") || !data["weapons"].is_object()) { std::cerr << "\nError getting weapons data"; return json(); }
		const json& weapons = data["weapons"]; // get list of weapons.

		if (weaponNames.empty()) {// only populate the first time.
			for (auto it = weapons.begin(); it != weapons.end(); ++it) {
				weaponNames.push_back(it.key());
			}
		}

		
		return GameData::getWeapon( // get weapon using name like normal.
				weaponNames[rng::getInt(0, weaponNames.size() - 1)]
				);
		
		
	
	}
	bool isMaxLevel() { return level  >= maxLevel; }
	MEvent<float>& getCooldownEvent() { return cooldownTickEvent; }

	virtual void update(float deltaTime)override {
		if (attackTimer <= 0) {
			Fire();
			attackTimer = attackSpeed;
		}
		attackTimer -= deltaTime;

		cooldownTickEvent.invoke(attackTimer);
	}


	const int& getDamage() const { return damage; }
	const float& getSpeed() const { return speed; }
	const float& getRange() const { return range; }
	const float& getProjectileRadius() const { return projRadius; }
	const float& getPierce() const { return pierce; }
	const float getAttackSpeed() const;
	const float& getAttackTimer() const { return attackTimer; }


	/// <summary>
	/// Must be defined in weapon subclass (projectile type must be assigned in the override).
	/// </summary>
	virtual void Fire() = 0;
	/// <summary>
	/// Handles incrementing weapon level, stats, and any special effects that occour upon leveling.
	/// </summary>
	virtual void UpdateStats() = 0;

	virtual void LevelUp() {
		++level;
		UpdateStats();
	}

	

	
	virtual void init() { LoadInfoFromJson(); }
	virtual void Destroy() {}
	const std::string getDescription() const { return description; }


protected:
	
	static constexpr unsigned char maxLevel = 9;
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

	sf::IntRect projectileRect;
	std::shared_ptr<SpriteAnimation> animation;


	virtual const json& LoadInfoFromJson();
	virtual const json& GetJsonData() {  return GameData::getWeapon(name); }


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

	/// <summary>
	/// Gets the ability id. Ability id is used as an index to get the ability sprite from the texture.
	/// </summary>
	/// <returns></returns>
	int getAbilityID() const { return abilityId; }
protected:

	int abilityId = -1;

	virtual const json& LoadInfoFromJson() override {
		const auto& json = WeaponBase::LoadInfoFromJson();
		if (json.contains("ability id")) abilityId = json["ability id"];
		else std::cerr << "\nAbility id not defined in json for :" << name;
			
		return json;
	}
	
	virtual void UpdateStats() override {}
	virtual const json& GetJsonData() override {  return GameData::getAbility(name); }


	
};









