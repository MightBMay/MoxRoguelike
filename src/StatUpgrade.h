#pragma once
#include <array>
#include <sstream>
#include "Player.h"
#include "JsonLoader.h"
enum StatType
{
	Empty,
	Health,
	Defence,
	Damage,
	Speed,
	HealthRegen,
	AttackSpeed,
	Gold

};

struct StatUpgrade {
public:
	static inline const std::string StatTypeToString[] =
	{ "empty", "health","health regen","defence", "damage", "speed", "attack speed", "gold" };
	const StatType type = StatType::Empty;
	explicit StatUpgrade(StatType type = StatType::Empty) :type(type) { 
		LoadFlat(StatTypeToString[type]);
		LoadMult(StatTypeToString[type]);
	}
	bool isMaxLevel() { return level >= 9; }
	void LevelUp() {
		if (level >= 9) return; // stop at level value 9 (level 10)
		level++;
		Player::getStats()->RecalculateStats();
	}
	int GetLevel() const { return level; }
	int getFlat() { return flatStats[level]; }
	float getMult() { return multStats[level]; }
	std::string GetStatString() {
		static std::ostringstream oss; // static to avoid re creating.
		oss.clear(); // clear and reset string to "".
		oss.str("");
		oss << "Grants + " << getFlat() << " | " << (getMult() - 1) * 100 << "%" << StatTypeToString[type];
		return oss.str();

	}

private:


	std::array<int, 10> flatStats = {};
	std::array<float, 10> multStats = {};
	int level = 0;

	void LoadFlat(std::string statType) {
		auto& json = GameDataLoader::getStatUpgrade(statType);

		if (json.contains("flat")) {
			const auto& jsonArray = json["flat"];
			if (jsonArray.is_array() && jsonArray.size() == flatStats.size()) {
				flatStats = jsonArray.get<std::array<int, 10>>(); // Direct assignment (C++17+)
			}
		}
	}
	void LoadMult(std::string statType) {
		auto& json = GameDataLoader::getStatUpgrade(statType);

		if (json.contains("mult")) {
			const auto& jsonArray = json["mult"];
			if (jsonArray.is_array() && jsonArray.size() == flatStats.size()) {
				multStats = jsonArray.get<std::array<float, 10>>(); // Direct assignment (C++17+)
			}
		}


	}


};