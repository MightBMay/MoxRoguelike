#pragma once
#include "pch.h"
#include "Player.h"
#include "Ninja_Q.h"


namespace playerClasses {
	class Ninja : public Player {
	public:
		Ninja() :Player("Ninja") {

		}

		void CreateAbilities(std::shared_ptr<sf::RenderWindow> window) override final {
			auto parentS = parent.lock();
			auto weaponQ = parentS->addComponent<Ninja_Q>();
			abilityHolder[0] = weaponQ;
			// when linking abilities, do in decreasing index 2-0, as we create the sprites
			// right to left to match the weapon sprites.
			playerUI->spriteBar->abilityBar->LinkAbility(2, weaponQ.lock());


			//auto weaponE = parentS->addComponent<Papyrmancer_Reap>();
			//abilityHolder[1] = weaponE;
			//playerUI->spriteBar->abilityBar->LinkAbility(1, weaponE.lock());


			//auto weaponR = parentS->addComponent<Papyrmancer_R>();
			//abilityHolder[2] = weaponR;
			//playerUI->spriteBar->abilityBar->LinkAbility(0, weaponR.lock());




		}
	};

}