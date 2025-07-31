#include "Player.h"
#include "GameObject.h"
#include <math.h>
#include "Projectile.h"
#include "Sow_Projectile.h"
#include "Reap_Projectile.h"
#include "Papyrmancer_Reap.h"
#include "UI_CooldownSprite.h"

void Player::update(float deltatime) {
	if (direction.lengthSquared() < 0.15f) return; //only move if direction held.
	parent->move( direction * speed * deltatime );
}

void Player::ProcessEvent(const std::optional<sf::Event>& event) {


	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
		switch (keyPressed->scancode) {
		case sf::Keyboard::Scancode::Up:
		case sf::Keyboard::Scancode::W:
			direction.y = -1;
			break;
		case sf::Keyboard::Scancode::Down:
		case sf::Keyboard::Scancode::S:
			direction.y = 1;
			break;
		case sf::Keyboard::Scancode::Left:
		case sf::Keyboard::Scancode::A:
			direction.x = -1;
			break;
		case sf::Keyboard::Scancode::Right:
		case sf::Keyboard::Scancode::D:
			direction.x = 1;
			break;
		default:
			break;
		}
	}

	else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
		switch (keyReleased->scancode) {
		case sf::Keyboard::Scancode::Up:
		case sf::Keyboard::Scancode::W:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
				direction.y = 1; // Switch to opposite if still pressed. prevents weird issues when pressing opposing directions at the same time. done for all 4.
			}
			else {
				direction.y = 0;
			}
			break;
		case sf::Keyboard::Scancode::Down:
		case sf::Keyboard::Scancode::S:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
				direction.y = -1;
			}
			else {
				direction.y = 0;
			}
			break;
		case sf::Keyboard::Scancode::Left:
		case sf::Keyboard::Scancode::A:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
				direction.x = 1;
			}
			else {
				direction.x = 0;
			}
			break;
		case sf::Keyboard::Scancode::Right:
		case sf::Keyboard::Scancode::D:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
				direction.x = -1;
			}
			else {
				direction.x = 0;
			}
			break;
		default:
			break;
		}
	}
	
	if (direction.lengthSquared() < 0)// only normalize if vector is non 0.
		direction = direction.normalized(); 
	UpdateFacingDirection();
	
}

void Player::UpdateFacingDirection() {
	if (direction.x == 0) return; // dont flip if input released.
	bool newFacingDirection = direction.x < 0; // signbit returns true if number is negative.
	if (newFacingDirection != facingDirection) { // only flip if facing direction is not the same as before.
		facingDirection = newFacingDirection; // store new facing direction
		parent->scaleObject(-1, 1); // flip player.
	}
}

void Player::CreateWeapons(
	std::shared_ptr<sf::RenderWindow> window,
	std::array<std::weak_ptr<WeaponBase>, 3>& weaponArray,
	std::array<std::shared_ptr<GameObject>, 3>& cdSprites) {

	sf::IntRect rect = sf::IntRect{ {0,0},{128,128} };		

	 auto weaponQ = parent->addComponent<Weapon<Sow_Projectile>>(
		std::make_shared<WeaponStats>(1, 500, 500, 32, 0.2f, 1),
		window);
	weaponArray[0] = std::static_pointer_cast<WeaponBase>(weaponQ.lock());
	cdSprites[0] = GameObject::Create("../assets/sprites/cardboard.png", rect);

	auto weaponE = parent->addComponent<Papyrmancer_Reap<Reap_Projectile>>(
		std::make_shared<WeaponStats>(1, 500, 500, 32, 1, 1),
		window);
	weaponArray[1] = std::static_pointer_cast<WeaponBase>(weaponE.lock());
	cdSprites[1] = GameObject::Create("../assets/sprites/cardboard.png", rect);


	auto weaponR = parent->addComponent<Weapon<Projectile>>(
		std::make_shared<WeaponStats>(1, 500, 500, 32, 0.2f, 1),
		window);
	weaponArray[2] = std::static_pointer_cast<WeaponBase>(weaponR.lock());
	cdSprites[2] = GameObject::Create("../assets/sprites/cardboard.png", rect);




	for (int i = 0; i < 3; i++) {
		auto& cdSprite = cdSprites[i];
		cdSprite->setPosition(64 + (136 * i), 64);
		cdSprite->setOrigin(64, 64);
		cdSprite->addComponent<UI_CooldownSprite>(window, weaponArray[i], rect);
		cdSprites[i] = cdSprite;

	}
}
