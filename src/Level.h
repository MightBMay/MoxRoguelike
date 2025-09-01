#pragma once
class GameObject;

class Level {
protected:
	/// <summary>
	/// maps int timestamp (seconds) to potential enemies spawning at that time.
	/// </summary>
	static inline std::map<int, std::vector<int> > enemyMap{};

	static inline std::vector<int>* currentEnemyOptions = nullptr;
	int subscriptionID = -1;


	std::shared_ptr<GameObject> Background;
	std::shared_ptr<GameObject> vignetteObject;

protected:
	virtual const json& LoadInfoFromJson(const json& data) {
		if (data.contains("enemyMap"))
			for (auto& [key_str, value_arr] : data["enemyMap"].items()) // for each key value pair defined in json,
				enemyMap.insert({ std::stoi(key_str), value_arr.get<std::vector<int>>() });// add it to the enemy map.


		if (enemyMap.empty()) {
			std::cerr << "\n Level was loaded with no enemies in enemyMap.";
		}
		else {
			currentEnemyOptions = &enemyMap.begin()->second;
		}

		return data;
	}

	virtual void Reset() {
		enemyMap.clear();
		currentEnemyOptions = nullptr;
	}

	virtual void CreateBackground();

public:
	Level(const std::string& levelName) {
		Reset();
		LoadInfoFromJson(GameData::getLevel(levelName));
		// will check for a change in enemy types from enemyMap every second.
		subscriptionID = second_Timer.getLoopEvent().subscribe([this]() {UpdateEnemyOptions(); });
		CreateBackground();
	}

	Level(const int& index) {
		Reset();
		LoadInfoFromJson(GameData::getLevelFromIndex(index));
		// will check for a change in enemy types from enemyMap every second.
		subscriptionID = second_Timer.getLoopEvent().subscribe([this]() {UpdateEnemyOptions(); });
		CreateBackground();
	}
	static inline std::vector<int>* GetCurrentEnemyOptions() {
		return currentEnemyOptions;
	}
	
	void UpdateEnemyOptions() {
		auto& it = enemyMap.find(elapsed_seconds);
		if (it != enemyMap.end()) {
			currentEnemyOptions = & enemyMap[elapsed_seconds];
		}
	}

	
};