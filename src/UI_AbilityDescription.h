#pragma once
#include "UI.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>

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
		
		
		rt->draw(*bgSprite);
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
	void OnHover() override;
	void OnHoverExit()override;
	void OnClick(const int button) override {}
	
private:
	
	static inline const sf::Vector2f spritePosition = sf::Vector2f(1528, 652);

	static inline std::shared_ptr<Renderable> renderable;
	
	static inline std::shared_ptr<sf::Sprite> sprite;
	static inline std::shared_ptr<sf::Text> text;
	static inline std::shared_ptr<sf::RenderTexture> rt;
	static inline sf::Texture bgTexture;
	static inline std::shared_ptr<sf::Sprite> bgSprite;
	// eventually move this into texture manager to cache it prob
	static inline std::shared_ptr<sf::Font> font;
	static inline const std::string panelBGPath = "../assets/sprites/cardboard.png";

};