#pragma once
#include <memory>
#include <array>

class GameObject;
class Player;
class UI_SpriteBarHolder;
class WeaponBase;
class StatUpgrade;
class ProgressBar;
enum StatType;

struct PlayerUI
{
public:

	PlayerUI(std::weak_ptr<Player> player);
		// ui bars.
	std::shared_ptr<UI_SpriteBarHolder> spriteBar;

	// ability stuff.
	std::shared_ptr <GameObject> abilityDescription;


	//health and healthbar
	std::shared_ptr<GameObject> healtBarObj;
	std::weak_ptr<ProgressBar> healthBar;


	void SetSpriteBarEnabled(int value);
	void UpdateHealthbar(int newHealth);
	void UI_AddStat(StatType stat);
	void UI_AddWeapon(int slotIndex, int weaponIndex);

private:

};

