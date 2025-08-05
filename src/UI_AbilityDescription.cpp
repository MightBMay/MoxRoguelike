#include "UI_AbilityDescription.h"

void UI_AbilityDescription::init() {
	renderable = std::make_shared<Renderable>(); 

	font = std::make_shared<sf::Font>();
	if (!font->openFromFile("../assets/fonts/amazon ember.ttf")) {
		std::cerr << "\nerror opening font"; return;
	}

	text = std::make_shared<sf::Text>(*font);

	rt = std::make_shared<sf::RenderTexture>(sf::Vector2u{390,300});
	sprite = std::make_shared<sf::Sprite>(rt->getTexture());

}

void UI_AbilityDescription::update(float deltaTime){}

void UI_AbilityDescription::OnHover() {}
void UI_AbilityDescription::OnHoverExit() {}