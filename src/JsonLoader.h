#pragma once


using json = nlohmann::json;
class GameData {
public:

	GameData() = delete;
	GameData(const GameData&) = delete;
	GameData& operator=(const GameData&) = delete;


	static bool loadAllData();


	static const json& getPlayerClass(const std::string& className);
	static const json& getPlayerClassFromIndex(const int& index);

	static const json& getEnemy(const std::string& enemyType);
	static const json& getEnemyFromIndex(const int& index);

	static const json& getWeapon(const std::string& weaponName);
	/// <summary>
	/// converts a weapon index to string, then uses the returned weapon name to get the actual weapon data.
	/// </summary>
	static const json& getWeaponFromIndex(const int weaponIndex);
	static const std::string getWeaponNameFromIndex(const int weaponIndex);

	//static const json& getProjectile(const std::string& projectileName);

	static const json& getAbility(const std::string& abilityName);

	static const json& getStatUpgrade(const std::string& statType);

	static const json& getStatFromIndex(const int statIndex);
	static const std::string getStatNameFromIndex(const int statIndex);


	static const json& getLevel(const std::string& levelName);
	static const json& getLevelFromIndex(const int& index);
	static const int getLevelCount();

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
	static inline std::shared_ptr<const json> s_levels = nullptr;

	static inline std::vector<std::string> levelIndexArray{};
};

