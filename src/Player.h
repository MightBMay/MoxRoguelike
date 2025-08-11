#pragma once
#include "Component.h"
#include "Weapon.h"



class Timer;
class GameObject;
class AbilityBar;
class WeaponBar;
class StatUpgradeBar;
class StatUpgrade;
class UI_AbilityDescription;
class ProgressBar;

struct PlayerStats {
public:

	PlayerStats(int maxHp, int speed):maxHp(maxHp), curHp(maxHp), speed(speed){}

	int& getMaxHp() { return maxHp; }
	int& getCurHp() { return curHp; }
	float& getSpeed(){ return speed; }
	const float getSize() { return size; }

	const int& MaxHp() const { return maxHp; }
	const int& CurHp() const { return curHp; }
	const float& Speed() const { return speed; }
	const float& Size() const { return size; }




private:
	int maxHp;
	int curHp;

	float speed = 300;

	const float size  = 32;

};


class Player : public Component, public std::enable_shared_from_this<Player> {
public:

	sf::Vector2f direction{ 0,0 };
	// 0 == right, 1 = left
	bool facingDirection = false;

	Player();

	static bool isVulnrable() { return _isVulnrable; }

	virtual void CreateAbilities(std::shared_ptr<sf::RenderWindow> window);
	std::shared_ptr<AbilityBar>& getAbilityBar() { return abilityBarUI; }
	std::shared_ptr<WeaponBar>& getWeaponBar() { return weaponBarUI; }

	virtual void AddWeapon(int slotIndex, int weaponIndex);
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