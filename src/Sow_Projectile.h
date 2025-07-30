#pragma once
#include "Projectile.h"
class EnemyManager;
class GameObject;
struct WeaponStats;

class Sow_Projectile : public Projectile {
	
public:
	Sow_Projectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats);

	void update(float deltaTime) override final;
	const std::string& getSpritePath() const final{
		return spritePath;
	}

	void init() override final;

	

protected:

private:
	static inline std::unordered_set<std::shared_ptr<GameObject>> hitEnemies;
	static inline std::string spritePath = "../assets/sprites/cardboard.png";
};