#include "UI_LevelUpSelection.h"
#include "UI_Button.h"
#include "Weapon.h"
#include "StatUpgrade.h"
#include "Global.h"
#include "Player.h"


Selection::Selection(std::shared_ptr<GameObject> object) {
	obj = object;
	button = obj->addComponent<UI_Button>(window);
	text = std::make_shared<sf::Text>(*font);
	renderTexture = std::make_shared<sf::RenderTexture>(size);
	obj->setSprite(renderTexture->getTexture(), {} );
	renderSprite = obj->getSprite();
	renderSprite->setColor(sf::Color::Cyan);
	renderable = obj->getRenderable();
	GameObjectManager::getInstance().addExternalRenderable(renderable, 200);
}

void Selection::OnClick() {
	if (auto weapon_S = weaponPtr.lock()) {
		// upgrade weapon here.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}

	else if (auto stat_S = statPtr.lock()) {
		stat_S->LevelUp();
		Player::getStats()->RecalculateStats();
	}

}

void Selection::UpdateOption() {
	auto originalPosition = obj->getPosition();
	text->setString(GetDescription());

	renderSprite->setTexture(*backgroundTexture);
	renderSprite->setPosition({ 0, 0 }); // reset position, as we modify the position of rendersprite for the final object
									// to change where it actually renders. 

	renderTexture->draw(*renderSprite);
	renderTexture->draw(*text);

	renderSprite->setTexture(renderTexture->getTexture());
	renderSprite->setPosition(originalPosition);



}

std::string Selection::GetDescription() {
	if (auto weapon_S = weaponPtr.lock())
		return weapon_S->getDescription();
	else if (auto stat_S = statPtr.lock())
		return stat_S->GetStatString();
	else return "";
}



void UI_LevelUpSelection::Show() {
	for (auto& selecton : Selections) {
		selecton->obj->setActive(true, true);
		selecton->button.lock()->SetEnabled(true);
	}
}

void UI_LevelUpSelection::Hide() {
	for (auto& selecton : Selections) {
		selecton->obj->setActive(false, true);
		selecton->button.lock()->SetEnabled(false);
	}
}



UI_LevelUpSelection::UI_LevelUpSelection(std::string fontPath) {
	
	
	Selection::backgroundTexture = TextureManager::getTexture("../assets/sprites/cardboard.png");
	Selection::backgroundTexture->setRepeated(true);
	Selection::font = std::make_shared<sf::Font>(fontPath);
	auto buttonSpacing = Selection::size.x +20;
	
	for (int i = 0; i < quantity; ++i) { 
		
		std::shared_ptr<GameObject> obj = GameObject::Create(200);
		obj->setPosition(postion + sf::Vector2f(i * buttonSpacing, 0));
		std::shared_ptr<Selection> selection = std::make_shared<Selection>(obj);
		
		Selections[i] = selection;
		selection->UpdateOption();
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

