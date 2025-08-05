#pragma once
#include "UI.h"
#include "Weapon.h"
#include "MEvent.h"
#include "UI_AbilityDescription.h"


/*
Example on how to make one.


void CreateTestButton(GameObjectManager& manager, std::shared_ptr<GameObject> obj) {
	sf::IntRect rect = sf::IntRect{ {0,0},{128,128} }; // make rect a varible or param, as the base and component want the same rect size
	obj = GameObject::Create(
		"../assets/sprites/cardboard.png",
		rect
	);
	obj->setPosition(512, 512);  set position and transforms of base
	obj->setOrigin(rect.size.x/2.0, rect.size.y/2.0);  set origin to the center of the texture.

	manager.setRenderLayer(obj, 100);  set render layer (100 is arbitrary)
										the component will go to the very next layer compared to the base

	std::shared_ptr<UI_CooldownSprite> sprite = // create component.
		obj->addComponent<UI_CooldownSprite>(
			window,
			weapon,
			rect,
			"../assets/sprites/shapes/bl_square_128.png").lock();


}


*/










class MSprite;

class UI_CooldownSprite : public UI_Element, public std::enable_shared_from_this<UI_CooldownSprite> {

public:
	
	/// <summary>
	/// Component that adds a white square over another sprite, that scales down to display a cooldowns timings.
	/// </summary>
	/// <param name="window"> all UI_Element subclasses need this for base constructor.</param>
	/// <param name="weapon"> used to susbscribe to weapon's cooldown event.</param>
	/// <param name="rect"> rect of the white sprite used to create a sprite.</param>
	/// <param name="path"> path of the sprite used to cover the parent gameobjects sprite. defaults to a white square.</param>
	UI_CooldownSprite(int index, std::shared_ptr<sf::RenderWindow> window, std::weak_ptr<WeaponBase> weapon, sf::IntRect rect,
		std::string path = "../assets/sprites/shapes/bl_square_128.png");
	virtual void init() override;
	virtual void OnHover() {
		UI_AbilityDescription::setDescription(weapon.lock());
		hoveringAny[spriteIndex] = true;
	}
	virtual void OnHoverExit() {
		hoveringAny[spriteIndex] = false;
		std::cout << "\n";
		for (bool value : hoveringAny) {
			std::cout << value << ", ";
				if (value)
					return; // if any of the other sprites are hovered, do not clear description ui.
			// prevents description from not rendering when flicking between cdSprites.
		}
		UI_AbilityDescription::clear();
	}
	virtual void OnClick(const Button& mouseButton) {}
	virtual void Destroy() override {
		
	}
	void CalculateScaleFromCooldown(float curTimer);




private:
	// used to check if any of the multiple cdsprites are being hovered.
	std::array<bool, 3>hoveringAny{};
	std::shared_ptr<GameObject> cooldownObject;
	int spriteIndex;
	int eventSubscriptionID=-1; // used to track and unsubscribe to events.
	std::weak_ptr<WeaponBase> weapon;
	float weaponMaxCooldown = 0;
	sf::Vector2f originalScale;

};