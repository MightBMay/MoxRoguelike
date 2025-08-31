#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Component.h"
using sf::Mouse::Button;

class GameObject;


class UI_Element : public Component {
public:

	UI_Element(const std::shared_ptr<sf::RenderWindow>& window) : window(window) {}
	virtual void init() override;
	virtual void update(float deltaTime);
	virtual void Destroy() {}
	virtual void SetEnabled(bool value) {
		 enabled = value;
	}

protected:
	/// <summary>
	/// Checks if the mouse is currently hovering over the UI_Element's rect.
	/// </summary>
	/// <returns> returns true if the object is being hovered over.</returns>
	virtual bool isHovering();


	virtual void onMouseClick();
	virtual void onKeyboardDown() {}
	virtual void onKeyboardHold() {}
	virtual void onKeyboardUp() {}
	
	
	virtual void OnHover() = 0;
	virtual void OnHoverExit() = 0;
	virtual void OnClick(const int button) = 0;

	bool enabled = true;

	std::shared_ptr<sf::RenderWindow> window;
private:
	
	bool wasHovering = false;




};