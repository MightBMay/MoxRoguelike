#include "UI_LevelUpSelection.h"
#include "UI_Button.h"
#include "GameObject.h"
#include "Weapon.h"
#include "StatUpgrade.h"
#include "Global.h"
#include "Renderable.h"

UI_LevelUpSelection::UI_LevelUpSelection(std::string fontPath) {
	
	
	Selection::backgroundTexture = TextureManager::getTexture("../assets/sprites/cardboard.png");
	Selection::font = std::make_shared<sf::Font>(fontPath);
	
	for (int i = 0; i < quantity; ++i) { 
		
		std::shared_ptr<GameObject> obj = GameObject::Create(200);
		std::shared_ptr<Selection> selection = std::make_shared<Selection>(obj);
		Selections[i] = selection;
	}
}

void UI_LevelUpSelection::UpdateOption(int index, std::weak_ptr<WeaponBase> weapon) {
	if (index < 0 || index >= quantity) return;
	auto& selection = Selections[index];
	selection->weaponPtr = weapon;


}
void UI_LevelUpSelection::UpdateOption(int index, std::weak_ptr<StatUpgrade> stat) {
	if (index < 0 || index >= quantity) return;
	auto& selection = Selections[index];
	selection->statPtr = stat;
}

void Selection::OnClick() {
	if (auto weapon_S = weaponPtr.lock()) {
		// upgrade weapon here.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}
		
	else if (auto stat_S = statPtr.lock()) {
		//stat_S->LevelUp(); figure out how to call player::playerstats::recalculatestats as well.
	}

}


Selection::Selection(std::shared_ptr<GameObject> object) {
	obj = object;
	button = obj->addComponent<UI_Button>(window);
	renderTexture = std::make_shared<sf::RenderTexture>(sf::Vector2u(300, 500));
	text = std::make_shared<sf::Text>(*font);
	renderSprite = std::make_shared<sf::Sprite>(*Selection::backgroundTexture);
	renderable = std::make_shared<Renderable>(renderSprite);
	//GameObjectManager::getInstance().addExternalRenderable(renderable,200);

}

void Selection::UpdateOption() {
	text->setString(GetDescription());

	renderSprite->setTexture(*backgroundTexture);
	renderTexture->draw(*renderSprite);
	renderTexture->draw(*text);

	renderSprite->setTexture(renderTexture->getTexture());



}

std::string Selection::GetDescription() {
	if (auto weapon_S = weaponPtr.lock())
		return weapon_S->getDescription();
	else if (auto stat_S = statPtr.lock())
		return stat_S->GetStatString();
	else return "";
}