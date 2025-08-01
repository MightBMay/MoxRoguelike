#pragma once
#include <SFML/Graphics.hpp>
#include "GameObjectPool.h"
#include "TimedDestroy.h"
#include <memory>
#include <vector>


class GameObject;

class EnemyManager {
public:
	static EnemyManager& getInstance();
	
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	

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

	std::vector<std::shared_ptr<GameObject>> GetWithinRange(sf::Vector2f position, float radius);
	std::vector<std::shared_ptr<GameObject>> GetWithinRange(sf::FloatRect rect);


private:
	EnemyManager() { hitboxVisuals.init(512, 0); };
	~EnemyManager() = default;

	static inline GameObjectPool<TimedDestroy> hitboxVisuals;
	std::vector<std::shared_ptr<GameObject>> enemyObjects_{};
};