#include "UI_LevelUpSelection.h"
#include "UI_Button.h"
#include "GameObject.h"
#include "Weapon.h"
#include "StatUpgrade.h"
#include "Global.h"
#include "MEvent.h"

UI_LevelUpSelection::UI_LevelUpSelection(std::string fontPath) {

	font = std::make_shared<sf::Font>(fontPath);
	for (int i = 0; i < quantity; ++i) { 
		std::shared_ptr<Selection> selection = std::make_shared<Selection>();
		std::shared_ptr<GameObject> obj = GameObject::Create(200);
		selection->obj = obj;
		selection->button = obj->addComponent<UI_Button>(window);
		selection->button.lock()->getOnClick().subscribe(selection, &Selection::OnClick);
		selection->renderTexture = std::make_shared<sf::RenderTexture>(sf::Vector2u(800, 500));
		selection->text = std::make_shared<sf::Text>(*font);
		
		
		
		Selections[i] = selection;
	}
}

void UI_LevelUpSelection::UpdateOption(int index, std::weak_ptr<WeaponBase> weapon) {
	if (index < 0 || index >= quantity) return;



}
void UI_LevelUpSelection::UpdateOption(int index, std::weak_ptr<StatUpgrade> weapon) {
	if (index < 0 || index >= quantity) return;

}

void Selection::OnClick() {
	if (auto weapon_S = weaponPtr.lock()) {
		// upgrade weapon here.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}
		
	else if (auto stat_S = statPtr.lock()) {
		//stat_S->LevelUp(); figure out how to call player::playerstats::recalculatestats as well.
	}

}

std::string Selection::GetDescription() {
	if (auto weapon_S = weaponPtr.lock())
		return weapon_S->getDescription();
	else if (auto stat_S = statPtr.lock())
		return stat_S->GetStatString();
	else return "";
}