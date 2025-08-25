#include "pch.h"
#include "UI_AbilityBar.h"
#include "GameObject.h"
#include "UI_CooldownSprite.h"
#include "UI_Button.h"
#include "Player.h"
#include "StatUpgrade.h"



void AbilityBar::Show() {
	enabled = true;
	for (auto& cdSprite : abilityCDSprites)
		if (cdSprite) {
			cdSprite->getRenderable()->enabled = true;
			cdSprite->getComponent<UI_CooldownSprite>()->SetEnabled(true);
		}
}
void AbilityBar::Hide() {
	enabled = false;
	for (auto& cdSprite : abilityCDSprites)
		if (cdSprite) {
			cdSprite->getRenderable()->enabled = false;
			cdSprite->getComponent<UI_CooldownSprite>()->SetEnabled(false);
		}
}

void AbilityBar::LinkAbility(int index, sf::IntRect& rect, std::shared_ptr<WeaponBase> wepBase,
	std::string path) {
	if (index > 3 || index < 0) return; // keep in bounds of array.
	// calculate offset based off how many abilities
	const sf::Vector2f offsetPerSprite = sf::Vector2f((68 * index)+24, 32);
	// make gameobject with desired sprite and rect.
	auto& abilitySprite = GameObject::Create(path, rect, 110);
	abilitySprite->setPosition(abilityBarPosition + offsetPerSprite); // set position accordingly
	abilitySprite->setOrigin(32, 32); // center icon.
	abilitySprite->addComponent<UI_CooldownSprite>(window, wepBase, rect); // create cd sprite (layer will be set to base object's layer +1)

	abilityCDSprites[index] = abilitySprite; // store sprite

}


void WeaponBar::Show() {
	enabled = true;
	for (auto& cdSprite : weaponCDSprites)
		if (cdSprite) {
			cdSprite->getRenderable()->enabled = true;
			cdSprite->getComponent<UI_CooldownSprite>()->SetEnabled(true);
		}
}
void WeaponBar::Hide() {
	enabled = false;
	for (auto& cdSprite : weaponCDSprites)
		if (cdSprite) {
			cdSprite->getRenderable()->enabled = false;
			cdSprite->getComponent<UI_CooldownSprite>()->SetEnabled(false);
		}
}

void WeaponBar::LinkWeapon(int index, int weaponIndex, std::shared_ptr<WeaponBase> wepBase) {
	
	if (index > 5 || index < 0) return; // keep in bounds of array.
	// calculate offset based off how many abilities
	const sf::Vector2f offsetPerSprite = sf::Vector2f(32 - (66 * index), 32);
	// make gameobject with desired sprite and rect.
	auto& weaponSprite = GameObject::Create();
	int rectPosX = (weaponIndex % columns) * spriteSize;
	int rectPosY = (weaponIndex / columns) * spriteSize;

	sf::IntRect rect = { {rectPosX, rectPosY},{64,64} };
	weaponSprite->setSprite(*weaponSpriteTexture, rect);
	GameObjectManager::getInstance().add(weaponSprite,110);
	weaponSprite->setPosition(weaponBarPosition + offsetPerSprite); // set position accordingly
	weaponSprite->setOrigin(32, 32); // center icon.
	weaponSprite->addComponent<UI_CooldownSprite>(window, wepBase, rect); // create cd sprite (layer will be set to base object's layer +1)

	if (!enabled) {
		weaponSprite->getRenderable()->enabled = false;
		weaponSprite->getComponent<UI_CooldownSprite>()->SetEnabled(false);
	}

	weaponCDSprites[index] = weaponSprite; // store sprite


}



void StatUpgradeBar::Show() {
	enabled = true;
	for (auto& statSprite : statCDSprites)
		if (statSprite) {
			statSprite->getRenderable()->enabled = true;
			statSprite->getComponent<UI_StatUpgradeSprite>()->SetEnabled(true);

		}
}
void StatUpgradeBar::Hide() {
	enabled = false;
	for (auto& statSprite : statCDSprites)
		if (statSprite) {
			statSprite->getRenderable()->enabled = false;
			statSprite->getComponent<UI_StatUpgradeSprite>()->SetEnabled(false);
		}
}

void StatUpgradeBar::LinkStat(std::shared_ptr<StatUpgrade> stat) {
	static constexpr int spriteWidth = 64;
	for (int index = 0; index < 6; ++index) {
		if (statCDSprites[index]) continue; // continue until you find a non null one.
		// calculate offset based off how many abilities
		const sf::Vector2f offsetPerSprite = sf::Vector2f(32 - (66 * index), 32);
		// make gameobject with desired sprite and rect.
		sf::IntRect rect = { {static_cast<int>(stat->type)* spriteWidth,0},{spriteWidth,spriteWidth} };
		auto& statSprite = GameObject::Create();
		statSprite->setSprite(*statUpgradeTexture, rect);
		GameObjectManager::getInstance().add(statSprite, 110);
		//statSprite->getSprite()->setColor(sf::Color::Red);
		statSprite->setPosition(statBarPosition + offsetPerSprite); // set position accordingly
		statSprite->setOrigin(32, 32); // center icon.
		statSprite->addComponent<UI_StatUpgradeSprite>(window, stat); // create cd sprite (layer will be set to base object's layer +1)

		if (!enabled) {
			statSprite->getRenderable()->enabled = false;
			statSprite->getComponent<UI_StatUpgradeSprite>()->SetEnabled(false);
		}

		statCDSprites[index] = statSprite; // store sprite
		return;
	}

	std::cerr << "\nNo space found in statCDSprites[].";

}


UI_SpriteBarHolder::UI_SpriteBarHolder(std::weak_ptr<Player> player) : player(player) {
	abilityBar = std::make_shared<AbilityBar>();
	weaponBar = std::make_shared<WeaponBar>();
	statBar = std::make_shared<StatUpgradeBar>();

	EnableBarUI(0); // default to showing abilities.

	for (int i = 0; i <3 ; ++i) {
		std::shared_ptr<GameObject> obj = GameObject::Create("../assets/sprites/cardboard.png", { {0,0},{32,20} }, 120);
														//34 = 32px button width, + 8 px gap. +8 to offset up 8 units
		obj->setPosition(position - sf::Vector2f(0,  ((2-i)*21.333f ) )); // as weird as this looks, it inverts the order the buttons	  
															      	 // show up so that it goes ability, weapon, statupgrade.
		std::shared_ptr<UI_Button> button = obj->addComponent<UI_Button>(window).lock();
		auto& clickEvent = button->getOnClick();
		clickEvent.subscribe([i,player]() { player.lock()->EnableBarUI(i); });
		barToggleButtons[i] = obj;
	}
	

}