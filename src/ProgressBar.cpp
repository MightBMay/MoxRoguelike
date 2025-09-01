#include "pch.h"
#include "GameObject.h"



ProgressBar::ProgressBar(
	sf::IntRect rect,
	std::string fillSpritePath,
	bool isHorizontal, float min, float max) :
	isHorizontal(isHorizontal), minValue(min), maxValue(max) {
	fillSprite = std::make_shared<MSprite>(fillSpritePath, rect);

	renderable = std::make_shared<Renderable>(fillSprite,fillSprite);
	
}


void ProgressBar::init() {
	auto parentS = parent.lock();
	startScale = parentS->getScale();

	sf::IntRect parentRect = parentS->getSprite()->getTextureRect();
	sf::IntRect fillRect = fillSprite->getTextureRect();

	// Calculate size differences
	float widthDifference = parentRect.size.x- fillRect.size.x;
	float heightDifference = parentRect.size.y- fillRect.size.y;

	// Calculate offset to center the fill sprite (half the difference)
	sf::Vector2f offset(widthDifference / 2.0f, heightDifference / 2.0f);

	// Apply the offset to the parent's position
	fillSprite->setPosition(parentS->getPosition() + offset);
	auto& manager = GameObjectManager::getInstance();
	manager.addExternalRenderable(renderable, manager.getRenderLayer(parentS)+1);
}

void ProgressBar::updateBar(int value) {
	curValue = normalizeFromRange(value, minValue, maxValue);

	if (isHorizontal)
		fillSprite->setScale({ startScale.x * curValue, startScale.y });
	//Technically functional, but would require shifting the origin based on texture rect, as it scales top down.
	//else
		//fillSprite->setScale({ startScale.x, startScale.y * curValue });

}
void ProgressBar::updateBar(float value) {
	curValue = normalizeFromRange(value, minValue, maxValue);

	if (isHorizontal)
		fillSprite->setScale({ startScale.x * curValue, startScale.y });
	//Technically functional, but would require shifting the origin based on texture rect, as it scales top down.
	//else
		//fillSprite->setScale({ startScale.x, startScale.y * curValue });

}

void ProgressBar::setFillColor(sf::Color& colour) { 
	if (fillSprite) fillSprite->setColor(colour); 
}

void ProgressBar::setRange(float min, float max) {
	minValue = min;
	maxValue = max;
}

void ProgressBar::setRange(int min, int max) {
	minValue = min;
	maxValue = max;
}

void ProgressBar::setRange(int max) {
	maxValue = max;
}