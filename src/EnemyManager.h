#pragma once
#include <SFML/Graphics.hpp>
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

	static void removeHitboxVisual(std::shared_ptr<GameObject>& hitbox);
	static void addHitboxVisual(std::shared_ptr<GameObject>& hitbox);
	int count() {
		return enemyObjects_.size();
	}

	std::vector<std::shared_ptr<GameObject>> GetWithinRange(sf::Vector2f position, float radius);
	std::vector<std::shared_ptr<GameObject>> GetWithinRange(sf::FloatRect rect);


private:
	EnemyManager() = default;
	~EnemyManager() = default;


	static inline std::vector<std::shared_ptr<GameObject>> hitboxVisuals;
	std::vector<std::shared_ptr<GameObject>> enemyObjects_;
};