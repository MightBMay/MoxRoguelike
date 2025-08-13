#include "UI.h"
#include "GameObject.h"
#include "Input.h"

void UI_Element::init() {  }

void UI_Element::onMouseClick() {

	if (Input::GetMouse(0)) // lmb
		OnClick(0);

	else if (Input::GetMouse(1)) //rmb
		OnClick(1);

	else if (Input::GetMouse(2)) //mmb
			OnClick(2);

}

void UI_Element::update(float deltaTime) {
	if (!parent->hasSprite()) return;
	bool currentlyHovering = isHovering();

	// if you weren't hovering last update() and are now,
	if (currentlyHovering && !wasHovering) {
		OnHover();
		wasHovering = true;
	}
	// if you were hovering last update() but aren't now,
	else if (!currentlyHovering && wasHovering) {

		OnHoverExit();
		wasHovering = false;
	}

	if (!wasHovering) return;
	onMouseClick();
	onKeyboardDown();
	onKeyboardHold();
	onKeyboardUp();


}



bool UI_Element::isHovering() {
	if (!enabled) return false;

	// get sprite from parent
	auto sprite = parent->getSprite();
	if (!sprite) {
		return false; // No sprite to hover over
	}

	// get mouse pos (world coordinates)
	auto desktopPos = sf::Mouse::getPosition(*window);
	auto mousePos = window->mapPixelToCoords(desktopPos, window->getDefaultView());



	// Get the sprite's global bounds (includes transform)
	sf::FloatRect spriteBounds = sprite->getGlobalBounds();

	// Check if mouse position is within bounds
	return spriteBounds.contains(mousePos);
}