#pragma once
#include <array>
#include <memory>
class WeaponBase;
class GameObject;
// used as the starting point of the ability bar.
constexpr sf::Vector2f basePosition = sf::Vector2f(0,0);

class AbilityBar {

public:
	std::array<std::weak_ptr<WeaponBase>, 3> weaponHolder;
	std::array<std::shared_ptr<GameObject>, 3> abilityCDSprites;


	void LinkWeapon(int index, sf::IntRect& rect, std::shared_ptr<WeaponBase> wepBase, std::string path = "../assets/sprites/cardboard.png") {
		if (index > 3 || index < 0) return; // keep in bounds of array.
		// calculate offset based off how many abilities
		const sf::Vector2f offsetPerAbility = sf::Vector2f( 64 + (136 * index), 64 ); 
		// make gameobject with desired sprite and rect.
		auto& cdSprite = GameObject::Create(path, rect); 
		cdSprite->setPosition(basePosition + offsetPerAbility); // set position accordingly
		cdSprite->setOrigin(64, 64); // center icon.
		GameObjectManager::getInstance().setRenderLayer(cdSprite, 100); // set gameobject base layer to 100 
		cdSprite->addComponent<UI_CooldownSprite>(window, wepBase, rect); // create cd sprite (layer will be set to base object's layer +1)

		weaponHolder[index] = wepBase; // store weapon
		abilityCDSprites[index] = cdSprite; // store sprite

	}

};