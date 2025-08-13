#pragma once
#include "UI.h"
#include "MEvent.h"

class UI_Button : public UI_Element {

public:
	explicit UI_Button(const std::shared_ptr<sf::RenderWindow>& window): UI_Element(window){}

	// done this way to allow access to Mevent with no reassignment.
	MEvent<>& getOnClick() { return onClickEvent; }

	virtual void OnHover() {
		auto sprite = parent->getSprite();
		sprite->setColor(sf::Color(192, 192, 192, 255));
	}
	virtual void OnHoverExit() {
		auto sprite = parent->getSprite();
		sprite->setColor(sf::Color(255, 255, 255, 255));
	}



	virtual void OnClick(const int button) override{
		onClickEvent.invoke();
	}

private:
	MEvent<> onClickEvent; // called on click.
};
