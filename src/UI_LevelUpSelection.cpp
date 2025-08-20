#include "UI_LevelUpSelection.h"
#include "UI_Button.h"
#include "Weapon.h"
#include "StatUpgrade.h"
#include "Global.h"
#include "Player.h"


Selection::Selection(std::shared_ptr<GameObject> object, std::weak_ptr<Player> player):player(player) {
	statType = Empty;
	obj = object;
	button = obj->addComponent<UI_Button>(window);
	text = std::make_shared<sf::Text>(*font);
	text->setOutlineThickness(2);
	renderTexture = std::make_shared<sf::RenderTexture>(size);
	obj->setSprite(renderTexture->getTexture(), {} );
	renderSprite = obj->getSprite();
	renderable = obj->getRenderable();
	GameObjectManager::getInstance().addExternalRenderable(renderable, 200);

}

void Selection::OnClick() {
	 
	if (weaponId >= 0)
		player.lock()->AddWeapon(weaponId);

	else if (statType != StatType::Empty)
		player.lock()->AddStat(statType);
		

}

void Selection::UpdateOption() {
	auto originalPosition = obj->getPosition();// since we re use the sprite to the 
											   // renderTexture, we gotta reposition.
	text->setString(GetDescription());

	renderSprite->setTexture(*backgroundTexture);
	renderSprite->setColor(sf::Color::Cyan); // DEBUG makes it easier to see.
	renderSprite->setPosition({ 0, 0 }); // draw background at 0,0

	renderTexture->draw(*renderSprite);
	renderTexture->draw(*text);
	renderTexture->display();
	renderSprite->setColor(sf::Color::White);
	renderSprite->setTexture(renderTexture->getTexture());
	renderSprite->setPosition(originalPosition);



}

std::string Selection::GetDescription() {
	if (weaponId >= 0) {
		std::string weaponName = GameDataLoader::getWeaponNameFromIndex(weaponId);
		auto& json = GameDataLoader::getWeapon(weaponName);
		if (json.contains("description"))
			return json["description"];
		else 
			std::cerr << "\n" << weaponName << " is missing description field.";

		
	}

	else if (statType != StatType::Empty) {
		std::string statName = GameDataLoader::getStatNameFromIndex(statType);
		
		auto& json = GameDataLoader::getStatUpgrade(statName);
		if (json.contains("description"))
			return json["description"];
		else
			std::cerr << "\n" << statName << " is missing description field";
	}
	
	return "";
}



void UI_LevelUpSelection::Show() {
	for (auto& selecton : Selections) {
		selecton->obj->setActive(true, true);
		selecton->button.lock()->SetEnabled(true);
	}
}
void UI_LevelUpSelection::ShowRandomSelections() {

	std::shuffle(shuffledWeapons.begin(), shuffledWeapons.end(), rng::getEngine());
	std::shuffle(shuffledStats.begin(), shuffledStats.end(), rng::getEngine());

	for (int i = 0; i < 3; ++i) {
		int upgradeType = rng::getInt(0, 1);
		if (upgradeType == 0) {
			// DEBUG /TODO find some way to exclude max leveled stats and weapons
			UpdateOption(i, shuffledWeapons[i]);
		}
		else {
			// DEBUG /TODO find some way to exclude max leveled stats and weapons
			UpdateOption(i, shuffledStats[i]);
		}
	}
}

void UI_LevelUpSelection::Hide() {
	for (auto& selecton : Selections) {
		selecton->obj->setActive(false, true);
		selecton->button.lock()->SetEnabled(false);
	}
}




UI_LevelUpSelection::UI_LevelUpSelection(std::weak_ptr<Player> player, std::string fontPath) {
	
	
	Selection::backgroundTexture = TextureManager::getTexture("../assets/sprites/cardboard.png");
	Selection::backgroundTexture->setRepeated(true);
	Selection::font = std::make_shared<sf::Font>(fontPath);
	auto buttonSpacing = Selection::size.x +20;
	
	for (int i = 0; i < quantity; ++i) { 
		
		std::shared_ptr<GameObject> obj = GameObject::Create(200);
		obj->setPosition(postion + sf::Vector2f(i * buttonSpacing, 0));
		std::shared_ptr<Selection> selection = std::make_shared<Selection>(obj,player);
		
		Selections[i] = selection;

		auto buttonS = selection->button.lock();
		buttonS->getOnClick().subscribe(selection, &Selection::OnClick);

		selection->UpdateOption();
	}

		// Precompute ranges as before
	int weaponCount = WeaponBase::WeaponListSize();
	weaponRange.resize(weaponCount);
	std::iota(weaponRange.begin(), weaponRange.end(), 0);

	statRange.resize(StatType::typeCount);
	for (int i = 0; i < StatType::typeCount; ++i) {
		statRange[i] = static_cast<StatType>(i);
	}

	// Initialize shuffled versions
	shuffledWeapons = weaponRange;
	shuffledStats = statRange;
	
}

void UI_LevelUpSelection::UpdateOption(int selectionIndex, int weaponIndex) {
	if (selectionIndex < 0 || selectionIndex >= quantity) return;
	auto& selection = Selections[selectionIndex];
	selection->weaponId = weaponIndex;
	selection->UpdateOption();


}
void UI_LevelUpSelection::UpdateOption(int selectionIndex, StatType statType) {
	if (selectionIndex < 0 || selectionIndex >= quantity) return;
	auto& selection = Selections[selectionIndex];
	selection->statType = statType;
	selection->UpdateOption();
}

