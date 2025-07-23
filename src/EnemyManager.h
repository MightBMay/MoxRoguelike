#pragma once

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
	int count() {
		return enemyObjects_.size();
	}
private:
	EnemyManager() = default;
	~EnemyManager() = default;



	std::vector<std::shared_ptr<GameObject>> enemyObjects_;
};