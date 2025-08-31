#pragma once
#include "pch.h"
#include "Player.h"
#include "PlayerUI.h"
#include "Weapon.h"
#include "Papyrmancer_Sow.h"
#include "Papyrmancer_Reap.h"
#include "Papyrmancer_R.h"
#include "Animation.h"


namespace playerClasses {


	class Papyrmancer : public Player {

	public:
		Papyrmancer() :Player("Papyrmancer") {

		}

		void CreateAbilities(std::shared_ptr<sf::RenderWindow> window) override final {
			auto parentS = parent.lock();
			auto weaponQ = parentS->addComponent<Papyrmancer_Sow>();
			abilityHolder[0] = weaponQ;
			// when linking abilities, do in decreasing index 2-0, as we create the sprites
			// right to left to match the weapon sprites.
			playerUI->spriteBar->abilityBar->LinkAbility(2, weaponQ.lock());


			auto weaponE = parentS->addComponent<Papyrmancer_Reap>();
			abilityHolder[1] = weaponE;
			playerUI->spriteBar->abilityBar->LinkAbility(1, weaponE.lock());


		   auto weaponR = parentS->addComponent<Papyrmancer_R>();
		   abilityHolder[2] = weaponR;
		   playerUI->spriteBar->abilityBar->LinkAbility(0, weaponR.lock());

			
			
			
		}

	};


}
