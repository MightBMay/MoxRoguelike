#include "Player.h"
#include "GameObject.h"
#include <math.h>

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

