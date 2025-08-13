#pragma once
#include <array>
#include <memory>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
class WeaponBase;
class GameObject;
class StatUpgrade;
class Player;
// used as the starting point of the ability bar.


class AbilityBar {
private:
	static constexpr sf::Vector2f abilityBarPosition = sf::Vector2f(1696, 1016);
	std::array<std::shared_ptr<GameObject>, 3> abilityCDSprites;
	bool enabled = true;
public:
	
	void Show();
	void Hide();

	void LinkAbility(int index, sf::IntRect& rect, std::shared_ptr<WeaponBase> wepBase,
		std::string path = "../assets/sprites/cardboard.png");

};

class WeaponBar {
	static constexpr sf::Vector2f weaponBarPosition = sf::Vector2f(1824, 1016);
	std::array<std::shared_ptr<GameObject>, 6> weaponCDSprites;
	bool enabled = false;
public:

	void Show();
	void Hide();

	void LinkWeapon(int index, std::shared_ptr<WeaponBase> wepBase, sf::IntRect& rect = sf::IntRect({ {0,0}, {64,64} }),
		std::string spritePath = "../assets/sprites/cardboard.png");
};

class StatUpgradeBar {
	static constexpr sf::Vector2f statBarPosition = sf::Vector2f(1824, 1016);
	std::array<std::shared_ptr<GameObject>, 6> statCDSprites;
	bool enabled = false;
public:
	void Show();
	void Hide();

	void LinkStat(std::shared_ptr<StatUpgrade> stat, sf::IntRect& rect = sf::IntRect({ {0,0}, {64,64} }),
		std::string spritePath = "../assets/sprites/cardboard.png");

};

class UI_SpriteBarHolder {
	static constexpr sf::Vector2f position = sf::Vector2f(1888, 1059.777f);
	std::array<std::shared_ptr<GameObject>, 3> barToggleButtons;
	std::weak_ptr<Player> player;

public:
	std::shared_ptr<AbilityBar> abilityBar;
	std::shared_ptr<WeaponBar> weaponBar;
	std::shared_ptr<StatUpgradeBar> statBar;


	UI_SpriteBarHolder(std::weak_ptr<Player> player);

	inline void EnableBarUI(int value) {
		switch (value)
		{
			case 0:
				abilityBar->Show();
				weaponBar->Hide();
				statBar->Hide();
				break;
			case 1:
				abilityBar->Hide();
				weaponBar->Show();
				statBar->Hide();
				break;
			case 2:
				abilityBar->Hide();
				weaponBar->Hide();
				statBar->Show();
				break;
			default:
				break;
		}
	}

};