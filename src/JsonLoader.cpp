#include "pch.h"

using json = nlohmann::json;

bool GameData::loadAllData() {
	try {
		std::ifstream file("../assets/gamedata/weapons.mox");
		if (file.is_open()) {
			s_weapons = std::make_shared<json>(json::parse(file));
			file.close();
		}

		
		file.open("../assets/gamedata/playerclasses.mox");
		if (file.is_open()) {
			s_player_classes = std::make_shared<json>(json::parse(file));
			file.close();
		}
		

	   // load StatUpgrades
		file.open("../assets/gamedata/statupgrades.mox");
		if (file.is_open()) {
			s_stat_upgrades = std::make_shared<json>(json::parse(file));
			file.close();
		}


	   // Load enemies (uncomment when ready)
		file.open("../assets/gamedata/enemies.mox");
		if (file.is_open()) {
			s_enemies = std::make_shared<json>(json::parse(file));
			file.close();
		}

		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load game data: " << e.what() << std::endl;
		return false;
	}
}

const json& GameData::getPlayerClass(const std::string& className) {
	return (*s_player_classes)["player_classes"][className];
}

const json& GameData::getEnemy(const std::string& enemyType) {
	return (*s_enemies)["enemies"][enemyType];
}

const json& GameData::getWeapon(const std::string& weaponName) {
	return (*s_weapons)["weapons"][weaponName];
}



	/// <summary>
	/// converts a weapon index to string, then uses the returned weapon name to get the actual weapon data.
	/// </summary>
const json& GameData::getWeaponFromIndex(const int weaponIndex) {
	return getWeapon((*s_weapons)["weapon indices"][std::to_string(weaponIndex)]);
}
 const std::string GameData::getWeaponNameFromIndex(const int weaponIndex) {
	return (*s_weapons)["weapon indices"][std::to_string(weaponIndex)];
}

 const json& GameData::getProjectile(const std::string& projectileName) {
	 return(*s_weapons)["projectiles"][projectileName];
 }

const json& GameData::getAbility(const std::string& abilityName) {
	return (*s_weapons)["abilities"][abilityName];
}

 const json& GameData::getStatUpgrade(const std::string& statType) {
	return (*s_stat_upgrades)["stat_upgrades"][statType];
}

const json& GameData::getStatFromIndex(const int statIndex) {
	return getStatUpgrade((*s_stat_upgrades)["stat indices"][std::to_string(statIndex)]);
}
const std::string GameData::getStatNameFromIndex(const int statIndex) {
	return (*s_stat_upgrades)["stat indices"][std::to_string(statIndex)];
}

