#pragma once
#include <array>
#include <memory>
class WeaponBase;
class GameObject;
class StatUpgrade;
// used as the starting point of the ability bar.


class AbilityBar {
private:
	static constexpr sf::Vector2f abilityBarPosition = sf::Vector2f(1528, 952);
	std::array<std::shared_ptr<GameObject>, 3> abilityCDSprites;
public:
	
	void Show() {
		for (auto& cdSprite : abilityCDSprites)
			if (cdSprite) {
				cdSprite->getRenderable()->enabled = true;
				cdSprite->getComponent<UI_CooldownSprite>()->SetEnabled(true);
			}
	}
	void Hide() {
		for (auto& cdSprite : abilityCDSprites)
			if (cdSprite) {
				cdSprite->getRenderable()->enabled = false;
				cdSprite->getComponent<UI_CooldownSprite>()->SetEnabled(false); 
			}
	}

	void LinkWeapon(int index, sf::IntRect& rect, std::shared_ptr<WeaponBase> wepBase,
		std::string path = "../assets/sprites/cardboard.png") {
		if (index > 3 || index < 0) return; // keep in bounds of array.
		// calculate offset based off how many abilities
		const sf::Vector2f offsetPerSprite= sf::Vector2f( 64 + (132 * index), 64 ); 
		// make gameobject with desired sprite and rect.
		auto& abilitySprite = GameObject::Create(path, rect, 110); 
		abilitySprite->setPosition(abilityBarPosition + offsetPerSprite); // set position accordingly
		abilitySprite->setOrigin(64, 64); // center icon.
		abilitySprite->setAsUI(true);
		abilitySprite->addComponent<UI_CooldownSprite>(window, wepBase, rect); // create cd sprite (layer will be set to base object's layer +1)

		abilityCDSprites[index] = abilitySprite; // store sprite

	}

};

class WeaponBar {
	static constexpr sf::Vector2f weaponBarPosition = sf::Vector2f(1528, 952);
	std::array<std::shared_ptr<GameObject>, 6> weaponCDSprites;

public:

	void Show() {
		for (auto& cdSprite : weaponCDSprites)
			if (cdSprite) {
				cdSprite->getRenderable()->enabled = true;
				cdSprite->getComponent<UI_CooldownSprite>()->SetEnabled(true);
			}
	}
	void Hide() {
		for (auto& cdSprite : weaponCDSprites)
			if (cdSprite) {
				cdSprite->getRenderable()->enabled = false;
				cdSprite->getComponent<UI_CooldownSprite>()->SetEnabled(false);
			}
	}

	void LinkWeapon(int index, std::shared_ptr<WeaponBase> wepBase, sf::IntRect& rect = sf::IntRect({ {0,0}, {64,64} }),
		std::string spritePath = "../assets/sprites/cardboard.png") {

			if (index > 5 || index < 0) return; // keep in bounds of array.
			// calculate offset based off how many abilities
			const sf::Vector2f offsetPerSprite = sf::Vector2f(32 + (66 * index), 32);
			// make gameobject with desired sprite and rect.
			auto& weaponSprite = GameObject::Create(spritePath, rect, 110);
			weaponSprite->setPosition(weaponBarPosition + offsetPerSprite); // set position accordingly
			weaponSprite->setOrigin(32, 32); // center icon.
			weaponSprite->setAsUI(true);
			weaponSprite->addComponent<UI_CooldownSprite>(window, wepBase, rect); // create cd sprite (layer will be set to base object's layer +1)

			weaponCDSprites[index] = weaponSprite; // store sprite


	}
};

class StatUpgradeBar {
	static constexpr sf::Vector2f statBarPosition = sf::Vector2f(1528, 600);
	std::array<std::shared_ptr<GameObject>, 6> statCDSprites;
public:
	void Show() {
		for (auto& statSprite : statCDSprites)
			if (statSprite) {
				statSprite->getRenderable()->enabled = true;
				statSprite->getComponent<UI_StatUpgradeSprite>()->SetEnabled(true);
				
			}
	}
	void Hide() {
		for (auto& statSprite : statCDSprites)
			if (statSprite) {
				statSprite->getRenderable()->enabled = false;
				statSprite->getComponent<UI_StatUpgradeSprite>()->SetEnabled(false);
			}
	}

	void LinkStat(std::shared_ptr<StatUpgrade> stat, sf::IntRect& rect = sf::IntRect({ {0,0}, {64,64} }),
		std::string spritePath = "../assets/sprites/cardboard.png") {
		for (int index = 0; index < 6; ++index){
			if (statCDSprites[index]) continue; // continue until you find a non null one.
			// calculate offset based off how many abilities
			const sf::Vector2f offsetPerSprite = sf::Vector2f(32 + (66 * index), 32);
			// make gameobject with desired sprite and rect.
			auto& statSprite = GameObject::Create(spritePath, rect, 110);
			statSprite->getSprite()->setColor(sf::Color::Red);
			statSprite->setPosition(statBarPosition + offsetPerSprite); // set position accordingly
			statSprite->setOrigin(32, 32); // center icon.
			statSprite->setAsUI(true);
			statSprite->addComponent<UI_StatUpgradeSprite>(window, stat); // create cd sprite (layer will be set to base object's layer +1)

			statCDSprites[index] = statSprite; // store sprite
			return;
		}

		std::cerr << "\nNo space found in statCDSprites[].";

	}

};