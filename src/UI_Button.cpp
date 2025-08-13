#include "UI_Button.h"
#include "GameObject.h"
void UI_Button::OnHover() {
	auto sprite = parent->getSprite();
	sprite->setColor(sf::Color(192, 192, 192, 255));
}
void UI_Button::OnHoverExit() {
	auto sprite = parent->getSprite();
	sprite->setColor(sf::Color(255, 255, 255, 255));
}