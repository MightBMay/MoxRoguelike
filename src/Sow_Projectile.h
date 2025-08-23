#pragma once
#include "Projectile.h"

class EnemyManager;
class GameObject;

class Sow_Projectile : public Projectile {
	
public:
	Sow_Projectile(sf::Vector2f direction, int* damage, float* speed, float* range, int* projectileSize, int pierce);
	static std::unordered_set<std::shared_ptr<GameObject>>& GetSowedEnemies() {
		return sowedEnemies;
	}
	static void ClearSowedEnemies() {
		sowedEnemies.clear();
	}
	void update(float deltaTime) override final;
	const std::string& getSpritePath() const final{
		return spritePath;
	}

	void CheckEnemies(sf::Vector2f curPos) override;
	void init() override final;

	

protected:
	
private:
	static inline const float duration = 7.5f;
	float remainingDuration = duration;
	static inline std::unordered_set<std::shared_ptr<GameObject>> sowedEnemies;
	static inline std::string spritePath = "../assets/sprites/projectiles/papyrmancer_sow.png";
};