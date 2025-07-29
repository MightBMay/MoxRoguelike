#pragma once
#include "UI.h"
#include "MEvent.h"

class UI_Button : public UI_Element {

public:
	explicit UI_Button(const std::shared_ptr<sf::RenderWindow>& window): UI_Element(window){}

	// done this way to allow access to Mevent with no reassignment.
	MEvent<>& getOnClick() { return onClickEvent; }

	virtual void OnHover() {
		std::cout << " hovering\n";

	}
	virtual void OnHoverExit() {
		std::cout << "stopped hovering\n";
	}



	virtual void OnClick(const Button& mouseButton) override{
		onClickEvent.invoke();
		std::cout << "invoked\n";
	}

private:
	MEvent<> onClickEvent; // called on click.
};
