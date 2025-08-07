#include "UI_AbilityDescription.h"

void UI_AbilityDescription::init() {
	renderable = std::make_shared<Renderable>();
	font = std::make_shared<sf::Font>();
	if (!font->openFromFile("../assets/fonts/amazon ember.ttf")) {
		std::cerr << "\nerror opening font"; return;
	}

	text = std::make_shared<sf::Text>(*font);
	text->setOutlineColor(sf::Color::Black);
	text->setOutlineThickness(2);
	rt = std::make_shared<sf::RenderTexture>(sf::Vector2u{390,300});
	sprite = std::make_shared<sf::Sprite>(rt->getTexture());

	bgTexture = *TextureManager::getTexture(panelBGPath);
	bgTexture.setRepeated(true);

	sf::Sprite tempSprite(bgTexture); // Construct with texture
	tempSprite.setTextureRect({ {0, 0}, {390, 300} }); // Set sub-rectangle
	bgSprite = std::make_shared<sf::Sprite>(tempSprite); // Copy into shared_ptr
	bgSprite->setColor(sf::Color(192, 192, 192));

}

void UI_AbilityDescription::update(float deltaTime){}

void UI_AbilityDescription::OnHover() {}
void UI_AbilityDescription::OnHoverExit() {}