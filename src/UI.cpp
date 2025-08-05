#include "UI.h"
#include "GameObject.h"


 void UI_Element::init() { parent->setAsUI(true); }

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
}

void UI_Element::ProcessEvent(const std::optional<sf::Event>& event){
	if (!parent->hasSprite()) return;
	if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
		ProcessMouseClick(*mouseClick);

	else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		ProcessKeyboardDown(*keyPressed);

	else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		ProcessKeyboardUp(*keyReleased);
	// can add keyboard input for ui if needed.



}


bool UI_Element::isHovering() {

    // get sprite from parent
    auto sprite = parent->getSprite();
    if (!sprite) {
        return false; // No sprite to hover over
    }

    // get mouse pos (world coordinates)
    auto desktopPos = sf::Mouse::getPosition(*window);
    auto mousePos = window->mapPixelToCoords(desktopPos, window->getDefaultView() );



    // Get the sprite's global bounds (includes transform)
    sf::FloatRect spriteBounds = sprite->getGlobalBounds();

    // Check if mouse position is within bounds
    return spriteBounds.contains(mousePos);
}