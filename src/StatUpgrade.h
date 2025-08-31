#pragma once

#include "Player.h"

enum StatType
{
	Empty,
	Health,
	HealthRegen,
	Defence,
	Damage,
	Speed,
	AttackSpeed,
	Gold,
	/// <summary>
	/// not an actual stat type, but can be used to get the number of stat types there are.
	/// keep in mind this value is the amount of types starting at 1, not 0.
	/// </summary>
	typeCount

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
	bool isMaxLevel() { return level >= maxLevel; }
	void LevelUp() {
		if (isMaxLevel()) return; // stop at level value 9 (level 10)
		level++;
		Player::getStats()->RecalculateStats();
	}
	int GetLevel() const { return level; }
	int getFlat() { return flatStats[level]; }
	float getMult() { return multStats[level]; }
	std::string GetStatString();

private:
	static constexpr int maxLevel = 9;

	std::array<int, 10> flatStats = {};
	std::array<float, 10> multStats = {};
	int level = 0;

	void LoadFlat(std::string statType);
	void LoadMult(std::string statType);


};