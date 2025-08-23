#pragma once

class GameObject;
class Player;
class UI_SpriteBarHolder;
class WeaponBase;
class StatUpgrade;
class ProgressBar;
enum StatType;
class UI_LevelUpSelection;

struct PlayerUI
{
public:

	PlayerUI(std::weak_ptr<Player> player);
		// ui bars.
	std::shared_ptr<UI_SpriteBarHolder> spriteBar;

	// ability stuff.
	std::shared_ptr <GameObject> abilityDescription;
	std::shared_ptr <UI_LevelUpSelection> levelUpUI;

	//health and healthbar
	std::shared_ptr<GameObject> healtBarObj;
	std::weak_ptr<ProgressBar> healthBar;


	void SetSpriteBarEnabled(int value);
	void UpdateHealthbar(int newHealth);
	void UI_AddStat(std::shared_ptr<StatUpgrade> stat);
	void UI_AddWeapon(int slotIndex, std::shared_ptr<WeaponBase> weapon);

private:

};

