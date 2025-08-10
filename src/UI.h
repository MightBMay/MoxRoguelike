#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Component.h"
using sf::Mouse::Button;

class GameObject;


class UI_Element : public virtual Component {
public:

	UI_Element(const std::shared_ptr<sf::RenderWindow>& window) : window(window) {}
	virtual void init() override;
	virtual void update(float deltaTime);
	virtual void Destroy() {}

protected:
	/// <summary>
/// Called upon hovering mouse over the UI element.
/// </summary>
/// <returns> returns true if the object is being hovered over.</returns>
	virtual bool isHovering();


	// make these all pure virtuals
	virtual void onMouseClick();
	virtual void onKeyboardDown() {}
	virtual void onKeyboardHold() {}
	virtual void onKeyboardUp() {}
	virtual void OnHover() = 0;
	virtual void OnHoverExit() = 0;
	virtual void OnClick(const int button) = 0;



private:
	std::shared_ptr<sf::RenderWindow> window;
	bool wasHovering = false;




};