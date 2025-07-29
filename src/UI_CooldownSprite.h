#pragma once
#include "UI.h"
#include "Weapon.h"
#include "MEvent.h"
class UI_CooldownSprite : public UI_Element {

public:
	template<typename ProjType>
	UI_CooldownSprite(std::shared_ptr<sf::RenderWindow> window, std::weak_ptr<Weapon<ProjType>> weaponWeak): 
	UI_Element(window){
		std::shared_ptr<Weapon> weapon = weaponWeak.lock();
		if (!weapon) return;
		eventSubscriptionID = weapon->getCooldownEvent().subscribe(this, &UI_CooldownSprite::CheckCooldown)
	}
	virtual void init() override {

	}
	virtual void OnHover() {}
	virtual void OnHoverExit() {}
	virtual void OnClick(const Button& mouseButton) {}
	virtual void Destroy() override {
		
	}




private:
	int eventSubscriptionID=-1; // used to track and unsubscribe to events.
	
	void CheckCooldown(float curTimer, float cooldown) {
		curTimer / cooldown;

	}
};