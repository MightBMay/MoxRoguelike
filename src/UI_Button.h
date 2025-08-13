#pragma once
#include "UI.h"
#include "MEvent.h"

class UI_Button : public UI_Element {

public:
	explicit UI_Button(const std::shared_ptr<sf::RenderWindow>& window): UI_Element(window){}

	// done this way to allow access to Mevent with no reassignment.
	MEvent<>& getOnClick() { return onClickEvent; }

	virtual void OnHover()override;
	virtual void OnHoverExit()override;



	virtual void OnClick(const int button) override{
		onClickEvent.invoke();
	}

private:
	MEvent<> onClickEvent; // called on click.
};
