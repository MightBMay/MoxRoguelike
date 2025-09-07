#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Component.h"
using sf::Mouse::Button;

class GameObject;

/// <summary>
/// Used as a base class for all UI. positions are WINDOW SIZE RELATIVE, place and scale all elements according 
/// to some constant, default render resolution (in my case, 2560x1440).
/// </summary>
class UI_Element : public Component {
public:

	UI_Element(const std::shared_ptr<sf::RenderWindow>& window) : window(window) {}
	virtual void init() override;
	virtual void update(float deltaTime);
	virtual void Destroy() {}
	virtual void SetEnabled(bool value) {
		 
		 if (!value && isHovering())
			 OnHoverExit(); // if u were hovering as it is set disabled, 
							 //this would be called, but the object is disabled so update() returns early
		 enabled = value;
	}

	virtual ~UI_Element() {
		//if (windowResizeEventID != -1)
			//windowResizeEvent.unsubscribe(windowResizeEventID);
	}

protected:
	/// <summary>
	/// Checks if the mouse is currently hovering over the UI_Element's rect.
	/// </summary>
	/// <returns> returns true if the object is being hovered over.</returns>
	virtual bool isHovering();

	virtual void SetRelativePosition(sf::Vector2u newSize);
	virtual void onMouseClick();
	virtual void onKeyboardDown() {}
	virtual void onKeyboardHold() {}
	virtual void onKeyboardUp() {}
	
	
	virtual void OnHover() = 0;
	virtual void OnHoverExit() = 0;
	virtual void OnClick(const int button) = 0;

	bool enabled = true;

	std::shared_ptr<sf::RenderWindow> window;
	sf::Vector2f relativePosition;

	//size_t windowResizeEventID = -1;

private:
	
	bool wasHovering = false;




};