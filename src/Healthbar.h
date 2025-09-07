#pragma once
#include "pch.h"
#include "ProgressBar.h"




class PlayerHealthBar : public ProgressBar {
	std::shared_ptr<sf::Text> text;
	std::shared_ptr<Renderable> textRenderable;
	sf::Vector2i rectSize;

public:
	PlayerHealthBar(sf::IntRect rect,
		std::string fillSpritePath = "../assets/sprites/shapes/bl_square_128.png",
		bool isHorizontal = true,
		float min = 0,
		float max = 1) :ProgressBar(rect, fillSpritePath, isHorizontal, min, max) {	
		rectSize = rect.size;
	}


	void init() override {
		ProgressBar::init();
		text = std::make_shared<sf::Text>(font);
		text->setOutlineThickness(2);
		text->setCharacterSize(24);
		updateBar(maxValue);

		auto pos = parent.lock()->getPosition();
		auto size = text->getLocalBounds().size;
		sf::Vector2f offset= {
			(rectSize.x - size.x) / 2.0f,
			(size.y) / 2.0f
		};
		text->setPosition(pos + offset);

		textRenderable = std::make_shared<Renderable>(text);
		GameObjectManager::getInstance().addExternalRenderable(textRenderable, 131);

	}
	void updateBar(float value)override {
		ProgressBar::updateBar(value);
		std::string& newStr = std::to_string(std::lround(value)) + " / " + std::to_string(std::lround(maxValue));
		text->setString(newStr);




	}

	void setRange(int maxV) override { maxValue = maxV; }




};