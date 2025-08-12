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
	const float getSize() { return size; }

	// non modifiable stats
	const int MaxHp() const;
	const int HealthRegen()const;
	const int Damage(int originalDamage)const;
	const float Speed() const;
	const float& Size() const;

	void AddUpgrade(StatType type);
	void RecalculateStats();


private:
	std::array< std::shared_ptr<StatUpgrade>,6>& statUpgrades;
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
	MEvent<int> onMaxHealthChange;
	Player();


	void HandleRegen(float deltatime);

	static bool isVulnrable() { return _isVulnrable; }

	virtual void CreateAbilities(std::shared_ptr<sf::RenderWindow> window);
	std::shared_ptr<AbilityBar>& getAbilityBar() { return abilityBarUI; }
	std::shared_ptr<WeaponBar>& getWeaponBar() { return weaponBarUI; }
	std::shared_ptr<StatUpgradeBar>& getStatBar() { return statUpgradeUI; }

	virtual void AddWeapon(int slotIndex, int weaponIndex);
	virtual void AddStat(StatType& type) { stats->AddUpgrade(type); }
	virtual void takeDamage(int _damage);
	void init()override;
	void update(float deltatime) override;
	virtual void Destroy() override {}

protected:
	// ability stuff.
	std::shared_ptr <GameObject> abilityDescription;
	std::shared_ptr<AbilityBar> abilityBarUI;
	std::array<std::weak_ptr<WeaponBase>, 3> abilityHolder;

	//health and healthbar
	std::shared_ptr<GameObject> healtBarObj;
	std::weak_ptr<ProgressBar> healthBar;
	Timer hitFlickerTimer{ hitFlickerDuration, false};


	//picked up weapons
	std::array<std::weak_ptr<WeaponBase>, 6> weaponHolder;
	std::array<int, 6> weaponIndices = { -1,-1,-1,-1,-1,-1 };
	std::shared_ptr<WeaponBar> weaponBarUI;

	//stat upgrades
	std::shared_ptr<StatUpgradeBar> statUpgradeUI;
	std::array<std::shared_ptr<StatUpgrade>, 6> statUpgradeHolder;


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