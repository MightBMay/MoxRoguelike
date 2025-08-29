#include "pch.h"
#include "GameObject.h"


void UI_Element::init() {  }

void UI_Element::onMouseClick() {
	if (Input::GetMouseUp(0)) // lmb
		OnClick(0);

	else if (Input::GetMouseUp(1)) //rmb
		OnClick(1);

	else if (Input::GetMouseUp(2)) //mmb
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

	if (!wasHovering) return;// only call the on___() if you are actually hovering the ui_element.
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

	// Get the sprite's global bounds (includes transform)
	sf::FloatRect spriteBounds = sprite->getGlobalBounds();

	// Check if mouse position is within bounds
	return spriteBounds.contains( // since UI is rendered with default view, not player's current view (camera), 
		window->mapPixelToCoords(Input::mousePos_Screen, window->getDefaultView()) // we need to map screenspace 
													//coords to world space with the default view.
	);
}