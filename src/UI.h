#pragma once
#include <SFML/Graphics.hpp>

#include "Component.h"
using sf::Mouse::Button;

class GameObject;


class UI_Element : public Component {
public:

	UI_Element(const std::shared_ptr<sf::RenderWindow>& window) : window(window) {}

	virtual void init() {}
	virtual void update(float deltaTime);
	virtual void Destroy() {}
	void ProcessEvent(const std::optional<sf::Event>& event) final;

protected:
	/// <summary>
/// Called upon hovering mouse over the UI element.
/// </summary>
/// <returns> returns true if the object is being hovered over.</returns>
	virtual bool isHovering();


	// make these all pure virtuals
	virtual void ProcessMouseClick(const sf::Event::MouseButtonPressed& click) {
		if (wasHovering) {
			switch (click.button)
			{
			case Button::Left:
				OnClick(Button::Left);
				break;

			case Button::Right:
				OnClick(Button::Right);
				break;

			case Button::Middle:
				OnClick(Button::Middle);
				break;


			default:
				break;
			}
		}
	}
	virtual void ProcessKeyboardDown(const sf::Event::KeyPressed& key) {}
	virtual void ProcessKeyboardUp(const sf::Event::KeyReleased& key) {}
	virtual void OnHover() = 0;
	virtual void OnHoverExit() = 0;
	virtual void OnClick(const Button& mouseButton) = 0;



private:
	std::shared_ptr<sf::RenderWindow> window;
	bool wasHovering = false;




};