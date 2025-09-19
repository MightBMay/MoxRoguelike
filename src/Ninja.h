#pragma once
#include "pch.h"
#include "Player.h"
#include "Ninja_Q.h"
#include "Ninja_E.h"


namespace playerClasses {
	class Ninja : public Player {

	private:
		float dashSpeed = 0;
		sf::Vector2f dashDir = {};
		bool isDashing = false;

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


			auto weaponE = parentS->addComponent<Ninja_E>(this);
			abilityHolder[1] = weaponE;
			playerUI->spriteBar->abilityBar->LinkAbility(1, weaponE.lock());


			//auto weaponR = parentS->addComponent<Papyrmancer_R>();
			//abilityHolder[2] = weaponR;
			//playerUI->spriteBar->abilityBar->LinkAbility(0, weaponR.lock());




		}


		void MovePlayer(float deltaTime) override {

			auto parentS = parent.lock();
			float usedSpeed;
			if (!isDashing) {
				// since we want to check the input direction before we dash (so we cant dash holding no dir)
				// we call UpdateMoveDirection in Ninja_E. with this if, we prevent double calling it.
				usedSpeed = stats->Speed();
				UpdateMoveDirection(); // handles input, and modifying the direction vector.
			}
			else
				usedSpeed = dashSpeed;

			sf::Vector2f moveAmount = direction * usedSpeed * deltaTime;
			parentS->move(moveAmount);
			HandleWalkSound(moveAmount.lengthSquared());
			playerView->setCenter(parentS->getPosition()); // set playerView center to player


		}

		void StartDash(float& speed) {
			dashSpeed = speed;
			dashDir = direction;
			isDashing = true;
		}

		void StopDash() {
			isDashing = false;
		}

	};

}