#pragma once
#include <array>
#include <sstream>
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
	explicit StatUpgrade(StatType type = StatType::Empty):type(type), stats(initializeStats(type)){}
	void LevelUp() {
		if (level >= 9) return; // stop at level value 9 (level 10)
		level++;
	}
	int GetLevel() const{ return level; }
	float GetValue() { return stats[level];}
	std::string GetStatString() {
		static std::ostringstream oss; // static to avoid re creating.
		oss.clear(); // clear and reset string to "".
		oss.str("");
		oss << "Grants + " << GetValue()<<" " << StatTypeToString[type];
		return oss.str();

	}

private:

	
	const std::array<float, 10> stats;
	int level = 0;

	static std::array<float, 10> initializeStats(StatType type) {
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


};