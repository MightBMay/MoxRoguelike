#pragma once

class WeaponBase;
class AbilityBase;
class GameObject;
class StatUpgrade;
class Player;
// used as the starting point of the ability bar.


class AbilityBar {
private:
	static constexpr sf::Vector2f abilityBarPosition = sf::Vector2f(1824, 1016);
	std::array<std::shared_ptr<GameObject>, 3> abilityCDSprites;
	bool enabled = true;

	static inline std::shared_ptr<sf::Texture> abilitySpriteTexture;
	static constexpr int spriteSize = 64;
	static inline int columns;

public:
	
	void Show();
	void Hide();

	void LinkAbility(int index, std::shared_ptr<AbilityBase> abilityBase);

	AbilityBar() {
		abilitySpriteTexture = TextureManager::getTexture("../assets/sprites/ui/weapon holder/ability sprites.png");
		columns = abilitySpriteTexture->getSize().x / spriteSize;
	}

};

class WeaponBar {
	static constexpr sf::Vector2f weaponBarPosition = sf::Vector2f(1824, 1016);
	std::array<std::shared_ptr<GameObject>, 6> weaponCDSprites;
	bool enabled = false;
	static inline std::shared_ptr<sf::Texture> weaponSpriteTexture;
	static constexpr int spriteSize = 64;
	static inline int columns;
public:

	void Show();
	void Hide();

	void LinkWeapon(int index, int weaponIndex, std::shared_ptr<WeaponBase> wepBase);


	WeaponBar() {
		weaponSpriteTexture = TextureManager::getTexture("../assets/sprites/ui/weapon holder/weapon sprites.png");
		columns = weaponSpriteTexture->getSize().x / spriteSize;
	}
};

class StatUpgradeBar {
	static constexpr sf::Vector2f statBarPosition = sf::Vector2f(1824, 1016);
	std::array<std::shared_ptr<GameObject>, 6> statCDSprites;
	bool enabled = false;
	// since we just move the rect for this, we only gotta get the texture once.
	static inline std::shared_ptr<sf::Texture> statUpgradeTexture;
public:
	void Show();
	void Hide();

	void LinkStat(std::shared_ptr<StatUpgrade> stat);
	StatUpgradeBar() {
		statUpgradeTexture = TextureManager::getTexture("../assets/sprites/UI/Weapon Holder/stat upgrade sprites.png");
	}

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