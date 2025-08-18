#pragma once
#include <array>
#include <sstream>
#include "Player.h"
enum StatType
{
	Empty,
	Health,
	Defence,
	Damage,
	Speed,
	HealthRegen,
	FireRate,
	Gold

};

struct StatUpgrade {
public:
	static inline const std::string StatTypeToString[] =
	{ "Empty", "Health","HealthRegen","Defence", "Damage", "Speed", "Fire Rate", "Gold" };
	const StatType type = StatType::Empty;
	explicit StatUpgrade(StatType type = StatType::Empty) :type(type), flatStats(initializeFlat(type)), multStats(initializeMult(type)) {}
	void LevelUp() {
		if (level >= 9) return; // stop at level value 9 (level 10)
		level++;
		Player::getStats()->RecalculateStats();
	}
	int GetLevel() const{ return level; }
	int getFlat() { return flatStats[level];}
	float getMult() { return multStats[level]; }
	std::string GetStatString() {
		static std::ostringstream oss; // static to avoid re creating.
		oss.clear(); // clear and reset string to "".
		oss.str("");
		oss << "Grants + " << getFlat()<<" | "<<(getMult()-1)*100<<"%" << StatTypeToString[type];
		return oss.str();

	}

private:

	
	const std::array<int, 10> flatStats;
	const std::array<float, 10> multStats;
	int level = 0;

	static std::array<int, 10> initializeFlat(StatType type) {
		switch (type) {
			case Health:
				return { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

			case HealthRegen:
				return { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
			
			case Defence:
				return { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
						
			case Damage:
				return { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
			
			case Speed:
				return { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };

			case FireRate:
				return { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };

			case Gold:
				return { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };

			default:
				return {};
		}
	}

	static std::array<float, 10> initializeMult(StatType type) {
		switch (type) {
			case Health:
				return {1,1,1,1,1,1,1,1,1,1};

			case HealthRegen:
				return { 1,1,1,1,1,1,1,1,1,1 };

			case Defence:
				return { 1,1,1,1,1,1,1,1,1,1 };

			case Damage:
				return { 1,1,1,1,1,1,1,1,1,1 };

			case Speed:
				return { 1,1,1,1,1,1,1,1,1,1 };

			case FireRate:
				return { 1,1,1,1,1,1,1,1,1,1 };

			case Gold:
				return { 1,1,1,1,1,1,1,1,1,1 };

			default:
				return { 1,1,1,1,1,1,1,1,1,1 };
		}
	}


};