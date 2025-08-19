#pragma once
#include "json.hpp"
#include <fstream>
#include <iostream>


using json = nlohmann::json;


class GameDataLoader {
public:

	GameDataLoader() = delete;
	GameDataLoader(const GameDataLoader&) = delete;
	GameDataLoader& operator=(const GameDataLoader&) = delete;


	static bool loadAllData() {
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
			// std::ifstream enemy_file("../assets/gamedata/enemies.json");
			// s_enemies = std::make_shared<json>(json::parse(enemy_file));

			return true;
		}
		catch (const std::exception& e) {
			std::cerr << "Failed to load game data: " << e.what() << std::endl;
			return false;
		}
	}


	static const json& getPlayerClass(const std::string& className) {
		return (*s_player_classes)["player_classes"][className];
	}

	static const json& getEnemy(const std::string& enemyType) {
		return (*s_enemies)["enemies"][enemyType];
	}

	static const json& getWeapon(const std::string& weaponName) {
		return (*s_weapons)["weapons"][weaponName];
	}

	static const json& getStatUpgrade(const std::string& statType) {
		return (*s_stat_upgrades)["stat_upgrades"][statType];
	}

	// Optional: Get direct access to the shared JSON objects
	static std::shared_ptr<const json> GetWeaponsData() { return s_weapons; }
	static std::shared_ptr<const json> GetPlayerClassesData() { return s_player_classes; }
	static std::shared_ptr<const json> GetEnemiesData() { return s_enemies; }
	static std::shared_ptr<const json> GetStatUpgradeData() { return s_stat_upgrades; }

private:
	// Shared JSON data (static and immutable after loading)
	static inline std::shared_ptr<const json> s_player_classes = nullptr;
	static inline std::shared_ptr<const json> s_enemies = nullptr;
	static inline std::shared_ptr<const json> s_weapons = nullptr;
	static inline std::shared_ptr<const json> s_stat_upgrades= nullptr;
};

