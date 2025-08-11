#include "ProgressBar.h"
#include "GameObject.h"



ProgressBar::ProgressBar(
	sf::IntRect rect,
	std::string fillSpritePath,
	bool isHorizontal, float min, float max) :
	isHorizontal(isHorizontal), minValue(min), maxValue(max) {
	fillSprite = std::make_shared<MSprite>(fillSpritePath, rect);

	renderable = std::make_shared<Renderable>(fillSprite);
	
}


void ProgressBar::init() {
	startScale = parent->getScale();

	sf::IntRect parentRect = parent->getSprite()->getTextureRect();
	sf::IntRect fillRect = fillSprite->getTextureRect();

	// Calculate size differences
	float widthDifference = parentRect.size.x- fillRect.size.x;
	float heightDifference = parentRect.size.y- fillRect.size.y;

	// Calculate offset to center the fill sprite (half the difference)
	sf::Vector2f offset(widthDifference / 2.0f, heightDifference / 2.0f);

	// Apply the offset to the parent's position
	fillSprite->setPosition(parent->getPosition() + offset);
	auto& manager = GameObjectManager::getInstance();
	manager.addExternalRenderable(renderable, manager.getRenderLayer(parent)+1);
}

void ProgressBar::updateBar(float value) {
	curValue = normalizeFromRange(value, minValue, maxValue);

	if (isHorizontal)
		fillSprite->setScale({ startScale.x * curValue, startScale.y });
	//Technically functional, but would require shifting the origin based on texture rect, as it scales top down.
	//else
		//fillSprite->setScale({ startScale.x, startScale.y * curValue });

}
