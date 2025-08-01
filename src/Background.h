#pragma once
#include "Component.h"
#include "Global.h"
#include "MSprite.h"

class BackgroundImage : public Component {


public:

	BackgroundImage() {}
	virtual void init()override;
	virtual void update(float deltaTime) override;
	virtual void Destroy()override {}
	virtual void ProcessEvent(const std::optional<sf::Event>& event) override {}

protected:
	const sf::Vector2f viewSize = playerView->getSize();
	std::shared_ptr<MSprite> sprite;
	
private:

};