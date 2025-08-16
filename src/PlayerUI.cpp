#include "PlayerUI.h"
#include "GameObject.h"
#include "Weapon.h"
#include "ProgressBar.h"
#include "StatUpgrade.h"
#include "UI_AbilityBar.h"
#include "UI_AbilityDescription.h"

PlayerUI::PlayerUI(std::weak_ptr<Player> player) {

	spriteBar = std::make_shared<UI_SpriteBarHolder>(player);



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
void PlayerUI::UI_AddStat(StatType stat) {

}
void PlayerUI::UI_AddWeapon(int slotIndex, int weaponIndex) {

}

void PlayerUI::SetSpriteBarEnabled(int value) { spriteBar->EnableBarUI(value); }