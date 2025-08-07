#pragma once
#include <array>
#include <memory>
class WeaponBase;
class GameObject;
// used as the starting point of the ability bar.
constexpr sf::Vector2f basePosition = sf::Vector2f(1528,952);

class AbilityBar {

public:

	std::array<std::weak_ptr<WeaponBase>, 3> weaponHolder;
	std::array<std::shared_ptr<GameObject>, 3> abilityCDSprites;


	void LinkWeapon(int index, sf::IntRect& rect, std::shared_ptr<WeaponBase> wepBase, std::string path = "../assets/sprites/cardboard.png") {
		if (index > 3 || index < 0) return; // keep in bounds of array.
		// calculate offset based off how many abilities
		const sf::Vector2f offsetPerAbility = sf::Vector2f( 64 + (132 * index), 64 ); 
		// make gameobject with desired sprite and rect.
		auto& abilitySprite = GameObject::Create(path, rect, 110); 
		abilitySprite->setPosition(basePosition + offsetPerAbility); // set position accordingly
		abilitySprite->setOrigin(64, 64); // center icon.
		abilitySprite->setAsUI(true);
		abilitySprite->addComponent<UI_CooldownSprite>(window, wepBase, rect); // create cd sprite (layer will be set to base object's layer +1)

		weaponHolder[index] = wepBase; // store weapon
		abilityCDSprites[index] = abilitySprite; // store sprite

	}

};