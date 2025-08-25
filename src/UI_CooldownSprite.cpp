#include "pch.h"
#include "UI_CooldownSprite.h"
#include "GameObject.h"
#include "StatUpgrade.h"






UI_CooldownSprite::UI_CooldownSprite(
	std::shared_ptr<sf::RenderWindow> window,  // all UI needs this to call base constructor
	std::weak_ptr<WeaponBase> weapon, // used to assign a weapon's cooldown to this component
	sf::IntRect rect, 
	std::string path) :
	UI_Element(window), weapon(weapon) {
	cooldownObject= GameObject::Create(
		path,
		rect);

}
//initialize the UI_CooldownSprite.
void UI_CooldownSprite::init(){

	auto shared = weapon.lock(); // lock weakPtr to weapon
	if (!shared) return; // if lock fails, exit.

	weaponMaxCooldown = shared->getAttackSpeed(); // get attack speed
	originalScale = parent->getScale(); // get original scale

	auto cdSprite = cooldownObject->getSprite(); // get actual MSprite
	cdSprite->setColor(sf::Color(192, 192, 192, 96)); // set translucent white, and repeating (in case base texture not big enough)
	cdSprite->SetRepeated(true);
	auto textureRect = cdSprite->getTextureRect().size; // get size

	
	cooldownObject->setOrigin( textureRect.x/2.0, +textureRect.y ); // set origin to center horizontally, and bottom vertically
																	// this covers the sprite, and scales the top edge downwards.

	// set position to the base sprite's position, plus offset to keep it covering the base sprite.
	cooldownObject->setPosition(parent->getPosition() + sf::Vector2f{ 0.0f,static_cast<float>(textureRect.y/2)});
	cooldownObject->setScale(originalScale.x, 0); // set y scale to 0, as it starts off cooldown.

	auto& manager = GameObjectManager::getInstance();
	manager.setRenderLayer(cooldownObject, manager.getRenderLayer(parent)+1 ); // set to layer above the parent object

	//subscribe checkCooldown method to the weapon's event.
	shared->getCooldownEvent().subscribe(shared_from_this(), &UI_CooldownSprite::CalculateScaleFromCooldown);

	UI_Element::init();

}


// normalizes weapon's attack time (gotten from weaponbase::cooldownTickEvent) to range 0-maxCooldown (gotten during init() )
// and assigns the scale of the cd sprite based on that.
void UI_CooldownSprite::CalculateScaleFromCooldown(float curTimer) {
	if (curTimer <= 0) return;
	cooldownObject->setScale({ originalScale.x, originalScale.y * curTimer / weaponMaxCooldown});
}







 void UI_StatUpgradeSprite::OnHover() {
	if (currentlyHovered && currentlyHovered != this) {
		currentlyHovered->OnHoverExit();
	}
	currentlyHovered = this;
	UI_AbilityDescription::setDescription(stat.lock()->GetStatString());
}


UI_StatUpgradeSprite::UI_StatUpgradeSprite(
	std::shared_ptr<sf::RenderWindow> window, std::weak_ptr<StatUpgrade> stat) :
	UI_Element(window), stat(stat) {


}
//initialize the UI_CooldownSprite.
void UI_StatUpgradeSprite::init() {

	UI_Element::init();

}
