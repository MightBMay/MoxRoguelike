#include "UI_LevelUpSelection.h"
#include "UI_Button.h"
#include "Weapon.h"
#include "StatUpgrade.h"
#include "Global.h"
#include "Player.h"


Selection::Selection(std::shared_ptr<GameObject> object, std::weak_ptr<Player> player) :player(player) {
	statType = Empty;
	obj = object;
	button = obj->addComponent<UI_Button>(window);
	text = std::make_shared<sf::Text>(*font);
	text->setOutlineThickness(2);
	renderTexture = std::make_shared<sf::RenderTexture>(size);
	obj->setSprite(renderTexture->getTexture(), {});
	renderSprite = obj->getSprite();
	renderable = obj->getRenderable();
	GameObjectManager::getInstance().addExternalRenderable(renderable, 200);

}

void Selection::OnClick() {

	if (weaponId >= 0)
		player.lock()->AddWeapon(weaponId);

	else if (statType != StatType::Empty)
		player.lock()->AddStat(statType);
	UI_LevelUpSelection::numRemainingLevels--;
	if (LevelUpUI == nullptr) { std::cerr << "\nError in assigning levelupui ptr, was still null when trying to access."; return; }
	LevelUpUI->HideIfNoLevels();


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
/// <summary>
/// Shuffles and assigns random weapons and stats to the Selection Options, then shows them to the screen.
/// </summary>
void UI_LevelUpSelection::ShowRandomSelections() {
	// shuffle weapons and stats to get random order with no repeats.
	std::shuffle(shuffledWeapons.begin(), shuffledWeapons.end(), rng::getEngine());
	std::shuffle(shuffledStats.begin(), shuffledStats.end(), rng::getEngine());
	auto& playerS = player.lock();

	// track current position in each shuffled array
	int weaponIndex = 0;
	int statIndex = 0;

	// generate 3 upgrade options 
	for (int i = 0; i < 3; ++i) {
		// choose between weapon or stat upgrade 
		int upgradeType = 0;// rng::getInt(0, 1);
		bool foundOption = false;

		if (upgradeType == 0) {

			// iterate shuffled weapons until one isnt max level
			while (weaponIndex < shuffledWeapons.size() && !foundOption) {
				int currentWeapon = shuffledWeapons[weaponIndex];
				// check if this weapon can be upgraded further
				if (!playerS->isWeaponMaxLevel(currentWeapon)) {
					UpdateOption(i, currentWeapon);
					foundOption = true; // escapes loop and cancels fallback checking of the other option type.
				}
				weaponIndex++; // move to next shuffled weapon
			}
		}
		else {

			// iterate shuffled stats until one isnt max level
			while (statIndex < shuffledStats.size() && !foundOption) {
				StatType currentStat = shuffledStats[statIndex];
				// Check if this stat can be upgraded further
				if (!playerS->isStatUpgradeMaxLevel(currentStat)) {
					UpdateOption(i, currentStat);
					foundOption = true; // escapes loop and cancels fallback checking of the other option type.
				}
				statIndex++; // move to next shuffled stat 
			}
		}

		// if chosen category from upgradeType has no more options, try to find a fill in from the other type.
		if (!foundOption) {
			if (upgradeType == 0) {
				// no weapon left, try stat.
				while (statIndex < shuffledStats.size() && !foundOption) {
					StatType currentStat = shuffledStats[statIndex];
					if (!playerS->isStatUpgradeMaxLevel(currentStat)) {
						UpdateOption(i, currentStat);
						foundOption = true;
					}
					statIndex++;
				}
			}
			else {
				// no stats left, try weapon
				while (weaponIndex < shuffledWeapons.size() && !foundOption) {
					int currentWeapon = shuffledWeapons[weaponIndex];
					if (!playerS->isWeaponMaxLevel(currentWeapon)) {
						UpdateOption(i, currentWeapon);
						foundOption = true;
					}
					weaponIndex++;
				}
			}
		}

		if (!foundOption) { // prob a good idea to log this.
			std::cout << "\nNo More Levelup options found.";
		}

	}

	Show(); // Display the UI with the selected upgrade options
}

void UI_LevelUpSelection::Hide() {
	for (auto& selecton : Selections) {
		selecton->obj->setActive(false, true);
		selecton->button.lock()->SetEnabled(false);
	}
}

void UI_LevelUpSelection::HideIfNoLevels() {
	if (numRemainingLevels <= 0) {
		Hide();
	}
	else {
		ShowRandomSelections();
	}


}



UI_LevelUpSelection::UI_LevelUpSelection(std::weak_ptr<Player> player, std::string fontPath) :player(player) {

	if (!player.lock()) std::cout << "no player";
	Selection::backgroundTexture = TextureManager::getTexture("../assets/sprites/cardboard.png");
	Selection::backgroundTexture->setRepeated(true);
	Selection::font = std::make_shared<sf::Font>(fontPath);
	auto buttonSpacing = Selection::size.x + 20;
	Selection::LevelUpUI = this;

	for (int i = 0; i < quantity; ++i) {

		std::shared_ptr<GameObject> obj = GameObject::Create(200);
		obj->setPosition(postion + sf::Vector2f(i * buttonSpacing, 0));
		std::shared_ptr<Selection> selection = std::make_shared<Selection>(obj, player);

		Selections[i] = selection;

		auto& onClickEvent = selection->button.lock()->getOnClick();
		onClickEvent.subscribe(selection, &Selection::OnClick);
		

		selection->UpdateOption();
	}

		// Precompute ranges as before
	int weaponCount = WeaponBase::WeaponListSize();
	weaponRange.resize(weaponCount);
	std::iota(weaponRange.begin(), weaponRange.end(), 0);

	statRange.resize(StatType::typeCount - 1);
	for (int i = 1; i < StatType::typeCount; ++i) {
		statRange[i - 1] = static_cast<StatType>(i);
	}

	// Initialize shuffled versions
	shuffledWeapons = weaponRange;
	shuffledStats = statRange;

	Hide();

}

void UI_LevelUpSelection::UpdateOption(int selectionIndex, int weaponIndex) {
	if (selectionIndex < 0 || selectionIndex >= quantity) return;
	std::cout << "\nweapon: " << weaponIndex;
	auto& selection = Selections[selectionIndex];
	selection->weaponId = weaponIndex;
	selection->UpdateOption();


}
void UI_LevelUpSelection::UpdateOption(int selectionIndex, StatType statType) {
	if (selectionIndex < 0 || selectionIndex >= quantity) return;
	std::cout << "\nstat: " << statType;
	auto& selection = Selections[selectionIndex];
	selection->statType = statType;
	selection->UpdateOption();
}

