#pragma once
#include <SFML/System/Vector2.hpp>
#include "GameObjectPool.h"
#include "TimedDestroy.h"
#include "GameObject.h"
#include "Global.h"
#include <memory>
#include <vector>
#include <functional>
#include <map>
#include "Utility.h"


class GameObject;
using std::function, std::shared_ptr, std::map;



class EnemyManager {

	static constexpr int minSpawnInterval = 6;
	static constexpr int maxSpawnInterval = 12;


public:
	static EnemyManager& getInstance();
	
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	

	static void SpawnEnemy(int index, int quantity, int level );
	static void SpawnEnemy(int index, int quantity);
	static void SpawnEnemy(int index);
	


	static void HandleSpawning(float deltaTime) {
		static float delay = 0;


		if (delay <= 0) {
			SpawnEnemy(0, rng::getInt(4,10));
			delay = rng::getInt(minSpawnInterval, maxSpawnInterval);
		}
		else {
			delay -= deltaTime;
		}
	}



	static inline int GetLevelFromTime() {
		float t = (elapsed_seconds / 60.0f) + 2.0f;
		return static_cast<int>(0.8f + 0.33f * t * t);
	}

	void add(std::shared_ptr<GameObject>);
	void remove(std::shared_ptr<GameObject>&);
	void remove(std::shared_ptr<GameObject>&, bool DestroyObject);
	static GameObjectPool<TimedDestroy>& getHitboxPool() { return hitboxVisuals; }
	static void removeHitboxVisual(std::shared_ptr<GameObject> obj) {
		hitboxVisuals.release(obj);
	}

	int count() {
		return enemyObjects_.size();
	}

	static std::vector<std::shared_ptr<GameObject>> getInRange(sf::Vector2f& position, float radius);
	static std::shared_ptr<GameObject> getFirstInRange(sf::Vector2f& position, float radius);
	static std::shared_ptr<GameObject> getClosest(const sf::Vector2f& position, const float cutoffRadiusSqr);


private:
	

	EnemyManager() { hitboxVisuals.init(512, 0); };
	~EnemyManager() = default;

	static inline GameObjectPool<TimedDestroy> hitboxVisuals;
	static inline std::vector<std::shared_ptr<GameObject>> enemyObjects_{};


	static const map<int, function<void(shared_ptr<GameObject>, int)>> EnemyList;


};