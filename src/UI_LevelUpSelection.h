#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>

class WeaponBase;
class StatUpgrade;
class GameObject;
class UI_Button;

struct Selection {
public:
	std::shared_ptr<GameObject> obj;
	std::weak_ptr<UI_Button> button;
	std::shared_ptr<sf::RenderTexture>renderTexture;
	std::shared_ptr<sf::Text> text;
	std::weak_ptr<WeaponBase> weaponPtr;
	std::weak_ptr<StatUpgrade> statPtr;

	std::string GetDescription();
	void OnClick();
	Selection() = default;
};

class UI_LevelUpSelection {
private:
	static constexpr int quantity = 3;
	std::shared_ptr<sf::Font> font;
	std::array<std::shared_ptr<Selection>, quantity> Selections;



public:
	UI_LevelUpSelection(std::string fontPath = "../assets/fonts/amazon ember.ttf");

	void UpdateOption(int index, std::weak_ptr<WeaponBase> weapon);
	void UpdateOption(int index, std::weak_ptr<StatUpgrade> weapon);


};