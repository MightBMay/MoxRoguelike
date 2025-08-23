#include "pch.h"
#include "PlayerUI.h"
#include "GameObject.h"
#include "Weapon.h"

#include "StatUpgrade.h"
#include "UI_AbilityBar.h"
#include "UI_AbilityDescription.h"
#include "UI_LevelUpSelection.h"

PlayerUI::PlayerUI(std::weak_ptr<Player> player) {

	spriteBar = std::make_shared<UI_SpriteBarHolder>(player);
	levelUpUI = std::make_shared<UI_LevelUpSelection>(player);


	sf::IntRect& healthBarRect = sf::IntRect{ {0,0},{256,32} };
	healtBarObj = GameObject::Create("../assets/sprites/cardboard.png", healthBarRect, 130);
	healtBarObj->getSprite()->SetRepeated(true);
	healtBarObj->setPosition(0, 1048);




	abilityDescription = GameObject::Create(120);
	abilityDescription->setPosition(1528, 652);

	abilityDescription->addComponent<UI_AbilityDescription>(window);
}

void PlayerUI::UpdateHealthbar(int newHealth) {
	healthBar.lock()->updateBar(newHealth);
}
void PlayerUI::UI_AddStat(std::shared_ptr<StatUpgrade> stat) {
	spriteBar->statBar->LinkStat(stat);
}
void PlayerUI::UI_AddWeapon(int slotIndex, std::shared_ptr<WeaponBase> weapon) {
	spriteBar->weaponBar->LinkWeapon(slotIndex, weapon);
}

void PlayerUI::SetSpriteBarEnabled(int value) { spriteBar->EnableBarUI(value); }