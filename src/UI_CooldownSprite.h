#pragma once
#include "UI.h"
#include "Weapon.h"
#include "MEvent.h"
#include "UI_AbilityDescription.h"

class StatUpgrade;
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
	UI_CooldownSprite(std::shared_ptr<sf::RenderWindow> window, std::weak_ptr<WeaponBase> weapon, sf::IntRect rect,
		std::string path = "../assets/sprites/shapes/bl_square_128.png");
	virtual void init() override;
	virtual void OnHover() {
		if (currentlyHovered && currentlyHovered!= this) {
			currentlyHovered->OnHoverExit();
		}
		currentlyHovered= this;
		UI_AbilityDescription::setDescription(weapon.lock());
	}

	virtual void OnHoverExit() {
		if (currentlyHovered== this) {
			currentlyHovered= nullptr;
			UI_AbilityDescription::clear();
		}
	}
	virtual void OnClick(const int Button) {}
	virtual void Destroy() override {
		
	}
	void CalculateScaleFromCooldown(float curTimer);

	virtual void SetEnabled(bool value) override{
		enabled = value;
		cooldownObject->getRenderable()->enabled = value;
	}


private:
	static inline UI_CooldownSprite* currentlyHovered = nullptr;
	std::shared_ptr<GameObject> cooldownObject;
	int spriteIndex;
	int eventSubscriptionID=-1; // used to track and unsubscribe to events.
	std::weak_ptr<WeaponBase> weapon;
	float weaponMaxCooldown = 0;
	sf::Vector2f originalScale;


};


class UI_StatUpgradeSprite :public UI_Element, public std::enable_shared_from_this<UI_StatUpgradeSprite> {

public:
	UI_StatUpgradeSprite(std::shared_ptr<sf::RenderWindow> window, std::weak_ptr<StatUpgrade> stat);
	virtual void init() override;
	virtual void OnHover() override;

	virtual void OnHoverExit() override {
		if (currentlyHovered == this) {
			currentlyHovered = nullptr;
			UI_AbilityDescription::clear();
		}
	}
	virtual void OnClick(const int Button) {}
	virtual void Destroy() override {}

private:
	static inline UI_StatUpgradeSprite* currentlyHovered = nullptr;
	int spriteIndex;
	std::weak_ptr<StatUpgrade> stat;
	sf::Vector2f originalScale;


};