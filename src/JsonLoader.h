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

			// Load player classes (uncomment when ready)
			// std::ifstream player_file("../assets/gamedata/playerClasses.json");
			// s_player_classes = std::make_shared<json>(json::parse(player_file));

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

	// Optional: Get direct access to the shared JSON objects
	static std::shared_ptr<const json> GetWeaponsData() { return s_weapons; }
	static std::shared_ptr<const json> GetPlayerClassesData() { return s_player_classes; }
	static std::shared_ptr<const json> GetEnemiesData() { return s_enemies; }

private:
	// Shared JSON data (static and immutable after loading)
	static inline std::shared_ptr<const json> s_player_classes = nullptr;
	static inline std::shared_ptr<const json> s_enemies = nullptr;
	static inline std::shared_ptr<const json> s_weapons = nullptr;
};

