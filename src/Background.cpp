#include "pch.h"
#include "GameObject.h"
void BackgroundImage::init() {
	sprite = parent.lock()->getSprite();
}
void BackgroundImage::update(float deltaTime)  {
	
	const sf::Vector2f viewCenter = playerView->getCenter();
	const float left = viewCenter.x - viewSize.x;
	const float top = viewCenter.y - viewSize.y;
	sprite->setTextureRect(
		sf::IntRect(
			{ static_cast<int>(left),  static_cast<int>(top) },
			{ static_cast<int>(viewSize.x * 2),  static_cast<int>(viewSize.y * 2) }
		)
	);
}