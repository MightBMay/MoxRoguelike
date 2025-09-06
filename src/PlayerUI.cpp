#include "pch.h"
#include "PlayerUI.h"
#include "GameObject.h"
#include "Weapon.h"

#include "StatUpgrade.h"
#include "Healthbar.h"
#include "UI_AbilityBar.h"
#include "UI_AbilityDescription.h"
#include "UI_LevelUpSelection.h"

PlayerUI::PlayerUI(std::weak_ptr<Player> player) {

	spriteBar = std::make_shared<UI_SpriteBarHolder>(player);
	levelUpUI = std::make_shared<UI_LevelUpSelection>(player);


	sf::IntRect& healthBarRect = sf::IntRect{ {0,0},{386,48} };
	healtBarObj = GameObject::Create("../assets/sprites/cardboard.png", healthBarRect, 130);
	healtBarObj->getSprite()->SetRepeated(true);
	healtBarObj->setPosition(sf::Vector2f{0,(window->getDefaultView().getSize().y - healthBarRect.size.y) });




	abilityDescription = GameObject::Create(120);
	//abilityDescription->setPosition(2138, 1376);
	abilityDescription->addComponent<UI_AbilityDescription>(window);
}

void PlayerUI::UpdateHealthbar(int newHealth) {
	healthBar.lock()->updateBar(newHealth);
}
void PlayerUI::UI_AddStat(std::shared_ptr<StatUpgrade> stat) {
	spriteBar->statBar->LinkStat(stat);
}
void PlayerUI::UI_AddWeapon(int slotIndex, int weaponIndex, std::shared_ptr<WeaponBase> weapon) {
	spriteBar->weaponBar->LinkWeapon(slotIndex, weaponIndex, weapon);
}

void PlayerUI::SetSpriteBarEnabled(int value) { spriteBar->EnableBarUI(value); }