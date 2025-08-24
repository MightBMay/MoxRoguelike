#pragma once
#include "pch.h"
#include "Player.h"
#include "PlayerUI.h"
#include "Weapon.h"
#include "Papyrmancer_Sow.h"
#include "Papyrmancer_Reap.h"
#include "Animation.h"


namespace playerClasses {


class Papyrmancer : public Player {

public:
	Papyrmancer() :Player("Papyrmancer") {

	}
	void init() override {
		Player::init();
		parent->addComponent<Animator>(parent, Animation());
	}
	void CreateAbilities(std::shared_ptr<sf::RenderWindow> window) override final {
		sf::IntRect abilityBarIconRect = sf::IntRect{ {0,0},{64,64} };

		auto weaponQ = parent->addComponent<Papyrmancer_Sow>();
		playerUI->spriteBar->abilityBar->LinkAbility(0, abilityBarIconRect, weaponQ.lock());


		auto weaponE = parent->addComponent<Papyrmancer_Reap>();
		playerUI->spriteBar->abilityBar->LinkAbility(1, abilityBarIconRect, weaponE.lock());


	   //auto weaponR = WeaponBase::CreateWeapon(1, parent);
	   //playerUI->spriteBar->abilityBar->LinkAbility(2, abilityBarIconRect, weaponR.lock());
	
		abilityHolder[0] = weaponQ;
		abilityHolder[1] = weaponE;
		//abilityHolder[2] = weaponR;
	}

private:
	std::string className;
	
};


}
