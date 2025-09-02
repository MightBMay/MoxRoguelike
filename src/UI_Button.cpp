#include "pch.h"
#include "UI_Button.h"
#include "GameObject.h"
void UI_Button::OnHover() {
	if (!enabled) return;
	auto sprite = parent.lock()->getSprite();
	sprite->setColor(sf::Color(192, 192, 192, 255));
}
void UI_Button::OnHoverExit() {
	auto sprite = parent.lock()->getSprite();
	sprite->setColor(sf::Color(255, 255, 255, 255));
}

void  UI_Button::OnClick(const int button){
	onClickEvent.invoke();
}

void UI_Button::SetEnabled(bool value){
	enabled = value;

	
	auto sprite = parent.lock()->getSprite();
	if (!sprite) return;

	if (!value)
		sprite->setColor(sf::Color(192, 192, 192, 255));
	else
		sprite->setColor(sf::Color::White);


}