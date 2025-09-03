#include "pch.h"
#include "UI_Button.h"
#include "GameObject.h"
void UI_Button::OnHover() {
	if (!enabled) return;
	auto sprite = parent.lock()->getSprite();
	sprite->setColor(sf::Color(192, 192, 192, 255));
	cursorStack.emplace(sf::Cursor::Type::Hand);
	window->setMouseCursor(sf::Cursor(cursorStack.top()));
}
void UI_Button::OnHoverExit() {
	auto sprite = parent.lock()->getSprite();
	sprite->setColor(sf::Color(255, 255, 255, 255));
	cursorStack.pop();
	if (cursorStack.empty()) 
		window->setMouseCursor(sf::Cursor(sf::Cursor::Type::Arrow));
	else
		window->setMouseCursor(sf::Cursor(cursorStack.top()));
}

void  UI_Button::OnClick(const int button){
	onClickEvent.invoke();
}

void UI_Button::SetEnabled(bool value){
	auto sprite = parent.lock()->getSprite();
	if (sprite) {

		if (!value) {
			sprite->setColor(sf::Color(192, 192, 192, 255));
			if (isHovering()) {
				OnHoverExit(); // if u were hovering as it is set disabled, 
								//this would be called, but the object is disabled so update() returns early
			}
		}
		else
			sprite->setColor(sf::Color::White);
	}

	enabled = value;
}