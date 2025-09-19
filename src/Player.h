#pragma once

#include "Weapon.h"
#include "PlayerUI.h"

class PlayerHealthbar;
class Component;
class Timer;
class GameObject;
class StatUpgrade;
enum StatType;

class UI_AbilityDescription;
class UI_SpriteBarHolder;
class PlayerUI;
class ProgressBar;

struct StatGroup {

private:
	int base = 0;
	int flat = 0;
	float mult = 1.0f;
	
public:
	StatGroup() {}
	StatGroup(int base) :base(base) {}
	StatGroup(int base, int flat, float mult) :base(base), flat(flat),mult(mult){}

	int& getBase() { return base; }
	int& getFlat() { return flat; }
	float& getMult() { return mult; }

	void UpdateValues(std::shared_ptr<StatUpgrade> upgrade);

	int evaluate() const {
		return static_cast<int>((base + flat) * mult);
	}
	int evaluate(int base) const{
		return static_cast<int>((base + flat) * mult);
	}
	float evaluate_f(float base) const {
		return (base + flat) * mult;
	}



};

struct PlayerStats {
public:

	PlayerStats( std::string className, std::array<std::shared_ptr<StatUpgrade>,6>& upgradeArray)
		:statUpgrades(upgradeArray) {
	}

	int& getMaxHp() { return maxHp.getBase(); }
	int& getCurHp() { return curHp; }
	int& getHealthRegen(){ return healthRegen.getBase(); }
	int& getSpeed(){ return speed.getBase(); }

	MEvent<int>& getMaxHpChangeEvent() { return onMaxHealthChange; }

	const float getSize() { return size; }

	/// <summary>
	/// Gets the non modifiable, fully evaluated (includes flat/mult bonuses) stat value.
	/// </summary>
	const int MaxHp() const;
	/// <summary>
	/// Gets the non modifiable, fully evaluated (includes flat/mult bonuses) stat value.
	/// </summary>
	const int HealthRegen()const;
	/// <summary>
	/// Gets the non modifiable, fully evaluated (includes flat/mult bonuses) stat value.
	/// </summary>
	const int Defence()const;
	/// <summary>
	/// Gets the non modifiable, fully evaluated (includes flat/mult bonuses) stat value.
	/// </summary>
	const int Damage(int originalDamage)const;
	/// <summary>
	/// Gets the non modifiable, fully evaluated (includes flat/mult bonuses) stat value.
	/// </summary>
	const float Speed() const;

	const float AttackSpeed(float originalSpeed)const;

	const float& Size() const;

	const int& CurXP() const;

	/// <summary>
	/// Adds an amount of XP to the players current xp, taking into account stat bonuses.
	/// </summary>
	/// <returns> returns the amount of levels gained</returns>
	int AddXp(int baseXp) {
		int levelsGained = 0;
		curXp += xp.evaluate(baseXp);
		while (curXp >= xpToNext) {
			curXp -= xpToNext;
			++levelsGained;
			// DEBUG/TODO:
			//xpToNext = GetNextXP();   get next level xp value.
		}
			// DEBUG/TODO:
		// re calculate stats here as well.
		return levelsGained;
	}
	/// <summary>
	///  Add a StatUpgrade to the player.
	/// </summary>
	/// <param name="type"> StatType of the upgrade</param>
	/// <returns>
	///		returns shared_ptr to the newly created upgrade if one of the given StatType was not found, and space was available.
	///		returns nullptr if upgrade already exists, or if no space for a new upgrade was found.
	/// </returns>
	std::shared_ptr<StatUpgrade> AddUpgrade(StatType type);
	void RecalculateStats();
	virtual void LoadInfoFromJson(const json& json);

protected:
	
private:
	std::array< std::shared_ptr<StatUpgrade>,6>& statUpgrades;
	MEvent<int> onMaxHealthChange{};


	int level = 1;
	int curXp = 0;
	int xpToNext = 100;

	int curHp;
	const float size = 32;

	StatGroup maxHp;
	StatGroup defence;
	StatGroup healthRegen;
	StatGroup speed;
	StatGroup damage;
	StatGroup attackSpeed;
	/// <summary>
	/// NOT the actual variable for xp value. that is curXP.
	/// </summary>
	StatGroup xp;
};


class Player : public Component, public std::enable_shared_from_this<Player> {
public:

	static const std::map<int, std::function<std::weak_ptr<Player>(std::shared_ptr<GameObject>)>> const playerClassList;
	static inline std::shared_ptr <sf::Texture> playerAtlasTexture;
	static std::shared_ptr<GameObject> CreatePlayerClass(int classIndex);
	static int PlayerClassListSize() { return playerClassList.size(); }

	sf::Vector2f direction{ 0,0 };
	// 0 == right, 1 = left
	bool facingDirection = false;


	Player(std::string className);


	void HandleRegen(float deltatime);

	const void AddXP(int baseXp);


	static bool isVulnrable() { return _isVulnrable; }
	void SetVulnrable(bool value) { 
		_isVulnrable = value;
	}

	static std::shared_ptr<PlayerStats> getStats() { return stats; }
	virtual void CreateAbilities(std::shared_ptr<sf::RenderWindow> window) = 0;
	void EnableBarUI(int value);

	const std::array<std::weak_ptr<WeaponBase>, 6>& getWeapons() const { return weaponHolder; }
	const std::array<int, 6>& getWeaponIndices() const { return weaponIndices; }
	virtual void AddWeapon(int weaponIndex);
	virtual void AddWeapon(int slotIndex, int weaponIndex);
	virtual void AddStat(StatType type);
	virtual void takeDamage(int _damage);
	void init()override;
	void update(float deltatime) override;
	virtual void UpdateMoveDirection();
	virtual void MovePlayer(float deltaTime);
	virtual void HandleWalkSound(float moveAmount);

	virtual void Destroy() override {}


	/// <returns>Returns the slot index the weapon is in, or -1 if not found. </returns>
	int hasWeapon(int weaponIndex);
	bool isWeaponMaxLevel(int weaponIndex);
	/// <returns>Returns the slot index the stat is in, or -1 if not found. </returns>
	int hasStat(StatType type);
	bool isStatUpgradeMaxLevel(StatType type);

protected:
	static inline std::shared_ptr<PlayerStats> stats = nullptr;
	std::string className = "";
	std::shared_ptr<PlayerUI> playerUI;
	Timer hitFlickerTimer{ hitFlickerDuration, false};

	/// <summary>
	/// used to be able to reference the current abilities.
	/// </summary>
	std::array<std::weak_ptr<WeaponBase>, 3> abilityHolder;


	/// <summary>
	/// used to be able to reference the current weapons
	/// </summary>
	std::array<std::weak_ptr<WeaponBase>, 6> weaponHolder{};
	// better way to track what specific weapons the player has.
	std::array<int, 6> weaponIndices = { -1,-1,-1,-1,-1,-1 }; 


	// actually OWNS the stat upgrades for the player.
	std::array<std::shared_ptr<StatUpgrade>, 6> statUpgradeHolder{};


	float healthRegenTimer = 1;

	void ResetHitFlicker() {
		parent.lock()->getSprite()->setColor(sf::Color::White);
		_isVulnrable = true;
	}

	virtual const json& LoadInfoFromJson(std::string className);

	FMOD::Studio::EventInstance* walkingEvent = nullptr;

private:
	static inline bool _isVulnrable = true;
	static constexpr float hitFlickerDuration = 0.125f;
	static constexpr sf::Color hitColour = sf::Color(255, 155, 155, 255);



};