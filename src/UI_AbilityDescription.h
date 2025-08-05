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
	virtual void Destroy() { TextureManager::releaseTexture(panelBGPath); }

	static void setDescription(std::shared_ptr<WeaponBase> weapon) {
		if (!weapon) { std::cerr << "\nInvalid weapon pointer (UI_AbiityDescription setDescription)"; return; }


		text->setString(weapon->getDescription());
		
		
		rt->clear(sf::Color::Transparent);	
		
		sf::Sprite bgSprite(bgTexture, { {0,0}, {390,300} });
		
		rt->draw(bgSprite);
		rt->draw(*text);
		rt->display();



		sprite->setTexture(rt->getTexture());
		sprite->setPosition(spritePosition);
		renderable->drawable = sprite;
		GameObjectManager::getInstance().addExternalRenderable(renderable, 121 ); // background for ability description is on 120.
	}

	static void clear() {
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

	static inline std::shared_ptr<Renderable> renderable;
	
	static inline std::shared_ptr<sf::Sprite> sprite;
	static inline std::shared_ptr<sf::Text> text;
	static inline std::shared_ptr<sf::RenderTexture> rt;
	static inline sf::Texture bgTexture;
	// eventually move this into texture manager to cache it prob
	static inline std::shared_ptr<sf::Font> font;
	static inline const std::string panelBGPath = "../assets/sprites/cardboard.png";

};