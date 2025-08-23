#include "pch.h"

using json = nlohmann::json;

bool GameDataLoader::loadAllData() {
	try {
		std::ifstream weapon_file("../assets/gamedata/weapons.json");
		s_weapons = std::make_shared<json>(json::parse(weapon_file));

		// load player classes 
		std::ifstream player_file("../assets/gamedata/playerclasses.json");
		s_player_classes = std::make_shared<json>(json::parse(player_file));

	   // load StatUpgrades
		std::ifstream stat_file("../assets/gamedata/statupgrades.json");
		s_stat_upgrades = std::make_shared<json>(json::parse(stat_file));


	   // Load enemies (uncomment when ready)
		std::ifstream enemy_file("../assets/gamedata/enemies.json");
		s_enemies = std::make_shared<json>(json::parse(enemy_file));

		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load game data: " << e.what() << std::endl;
		return false;
	}
}

const json& GameDataLoader::getPlayerClass(const std::string& className) {
	return (*s_player_classes)["player_classes"][className];
}

const json& GameDataLoader::getEnemy(const std::string& enemyType) {
	return (*s_enemies)["enemies"][enemyType];
}

const json& GameDataLoader::getWeapon(const std::string& weaponName) {
	return (*s_weapons)["weapons"][weaponName];
}



	/// <summary>
	/// converts a weapon index to string, then uses the returned weapon name to get the actual weapon data.
	/// </summary>
const json& GameDataLoader::getWeaponFromIndex(const int weaponIndex) {
	return getWeapon((*s_weapons)["weapon indices"][std::to_string(weaponIndex)]);
}
 const std::string GameDataLoader::getWeaponNameFromIndex(const int weaponIndex) {
	std::cout << "\nindex: " << weaponIndex;
	return (*s_weapons)["weapon indices"][std::to_string(weaponIndex)];
}

const json& GameDataLoader::getAbility(const std::string& abilityName) {
	return (*s_weapons)["abilities"][abilityName];
}

 const json& GameDataLoader::getStatUpgrade(const std::string& statType) {
	return (*s_stat_upgrades)["stat_upgrades"][statType];
}

const json& GameDataLoader::getStatFromIndex(const int statIndex) {
	return getStatUpgrade((*s_stat_upgrades)["stat indices"][std::to_string(statIndex)]);
}
const std::string GameDataLoader::getStatNameFromIndex(const int statIndex) {
	return (*s_stat_upgrades)["stat indices"][std::to_string(statIndex)];
}

