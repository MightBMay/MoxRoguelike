#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "GameObject.h" // for the life of me i cannot figure out why i can't move this to the source file
						// but it works when it's in the header for whatever reason so we take those.
class WeaponBase;
class StatUpgrade;
class GameObject;
class GameObjectManager;
class UI_Button;
class Renderable;


struct Selection {
public:
	static constexpr sf::Vector2u size = sf::Vector2u(500, 700);
	static inline std::shared_ptr<sf::Texture> backgroundTexture;
	static inline std::shared_ptr<sf::Font> font;
	std::shared_ptr<GameObject> obj;
	std::weak_ptr<UI_Button> button;
	
	std::shared_ptr<sf::RenderTexture>renderTexture;
	std::shared_ptr<sf::Sprite> renderSprite;
	std::shared_ptr<Renderable> renderable;

	std::shared_ptr<sf::Text> text;

	std::weak_ptr<WeaponBase> weaponPtr;
	std::weak_ptr<StatUpgrade> statPtr;

	std::string GetDescription();


	void UpdateOption();

	void OnClick();
	Selection(std::shared_ptr<GameObject> object);
};

class UI_LevelUpSelection {
private:
	static constexpr sf::Vector2f postion = sf::Vector2f(200, 175);
	static constexpr int quantity = 3;
	std::shared_ptr<sf::Font> font;
	std::array<std::shared_ptr<Selection>, quantity> Selections;




public:
	UI_LevelUpSelection(std::string fontPath = "../assets/fonts/amazon ember.ttf");

	void UpdateOption(int index, std::weak_ptr<WeaponBase> weapon);
	void UpdateOption(int index, std::weak_ptr<StatUpgrade> weapon);


};