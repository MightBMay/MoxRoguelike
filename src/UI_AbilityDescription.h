#pragma once
#include "UI.h"
#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include "Renderable.h"
#include "MSprite.h"

class UI_AbilityDescription : public UI_Element {
	

public:
	UI_AbilityDescription(const std::shared_ptr<sf::RenderWindow>& window):UI_Element(window){}

	virtual void init() override;
	virtual void update(float deltaTime);
	virtual void Destroy() {}

	void setDescription(std::shared_ptr<WeaponBase> weapon) {
		if (!weapon) { std::cerr << "\nInvalid weapon pointer (UI_AbiityDescription setDescription)"; return; }
		text->setString(weapon->getDescription());
		
		
		rt->clear(sf::Color::Transparent);
		rt->draw(*text);
		rt->display();

		sprite->setTexture(rt->getTexture());
		sprite->setPosition(spritePosition);
		renderable->drawable = sprite;
		auto& manager = GameObjectManager::getInstance();
		manager.addExternalRenderable(renderable, manager.getRenderLayer(parent)+1 );
	}

	void clear() {
		text->setString("");
		GameObjectManager::getInstance().removeExternalRenderable(renderable);
	}

protected:

	// overridden to do nothing when clicked.
	void ProcessMouseClick(const sf::Event::MouseButtonPressed& click)override {}
	void ProcessKeyboardDown(const sf::Event::KeyPressed& key)override {}
	void ProcessKeyboardUp(const sf::Event::KeyReleased& key)override {}
	void OnClick(const Button& mouseButton) {}

	void OnHover() override;
	void OnHoverExit()override;
	
private:
	
	static inline const sf::Vector2f spritePosition = sf::Vector2f(1528, 652);

	std::shared_ptr<Renderable> renderable{};
	
	std::shared_ptr<sf::Sprite> sprite;
	std::shared_ptr<sf::Text> text;
	std::shared_ptr<sf::RenderTexture> rt;
	// eventually move this into texture manager to cache it prob
	std::shared_ptr<sf::Font> font; 


};