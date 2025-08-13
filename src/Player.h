#pragma once
#include "Component.h"
#include "Weapon.h"



class Timer;
class GameObject;
class AbilityBar;
class WeaponBar;
class StatUpgradeBar;
class StatUpgrade;
enum StatType;
class UI_AbilityDescription;
class UI_SpriteBarHolder;
class ProgressBar;

struct StatGroup {

private:
	int base = 0;
	int flat = 0;
	float mult = 1.0f;
	
public:
	StatGroup() {}
	StatGroup(int base) :base(base) {}

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



};

struct PlayerStats {
public:

	PlayerStats( std::array<std::shared_ptr<StatUpgrade>,6>& upgradeArray, int maxHp,int defence, int healthRegen, int speed )
		:statUpgrades(upgradeArray), maxHp(maxHp), curHp(maxHp/2),defence(defence), healthRegen(healthRegen), speed(speed) {}

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

	const float& Size() const;

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


private:
	std::array< std::shared_ptr<StatUpgrade>,6>& statUpgrades;
	MEvent<int> onMaxHealthChange{};
	int curHp;
	const float size = 32;
	StatGroup maxHp;
	StatGroup defence;
	StatGroup healthRegen;
	StatGroup speed;
	StatGroup damage;
};


class Player : public Component, public std::enable_shared_from_this<Player> {
public:

	sf::Vector2f direction{ 0,0 };
	// 0 == right, 1 = left
	bool facingDirection = false;


	Player();


	void HandleRegen(float deltatime);

	static bool isVulnrable() { return _isVulnrable; }

	virtual void CreateAbilities(std::shared_ptr<sf::RenderWindow> window);
	std::shared_ptr<UI_SpriteBarHolder> getSpriteBarUI() { return spriteBar; }
	void EnableBarUI(int value);

	virtual void AddWeapon(int slotIndex, int weaponIndex);
	virtual void AddStat(StatType type);
	virtual void takeDamage(int _damage);
	void init()override;
	void update(float deltatime) override;
	virtual void Destroy() override {}

protected:
	// ui bars.
	std::shared_ptr<UI_SpriteBarHolder> spriteBar;

	// ability stuff.
	std::shared_ptr <GameObject> abilityDescription;
	std::array<std::weak_ptr<WeaponBase>, 3> abilityHolder;


	//picked up weapons
	std::array<std::weak_ptr<WeaponBase>, 6> weaponHolder;
	std::array<int, 6> weaponIndices = { -1,-1,-1,-1,-1,-1 }; // used to track what weapon the player has


	//stat upgrades

	std::array<std::shared_ptr<StatUpgrade>, 6> statUpgradeHolder;

	//health and healthbar
	std::shared_ptr<GameObject> healtBarObj;
	std::weak_ptr<ProgressBar> healthBar;
	Timer hitFlickerTimer{ hitFlickerDuration, false};




	float healthRegenTimer = 1;

	void ResetHitFlicker() {
		parent->getSprite()->setColor(sf::Color::White);
		_isVulnrable = true;
	}


private:
	static inline bool _isVulnrable = true;
	static constexpr float hitFlickerDuration = 0.125f;
	static constexpr sf::Color hitColour = sf::Color(255, 155, 155, 255);
	std::shared_ptr<PlayerStats> stats = nullptr;
	void UpdateFacingDirection();


};