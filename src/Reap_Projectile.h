#pragma once
#pragma once
#include "Projectile.h"
class EnemyManager;
class GameObject;
struct WeaponStats;

class Reap_Projectile : public Projectile {

public:
	Reap_Projectile(sf::Vector2f direction, std::weak_ptr<WeaponStats> stats);
	void update(float deltaTime) override final;
	const std::string& getSpritePath() const final {
		return spritePath;
	}

	void init() override final;



protected:

private:
	const float duration = 7.5f;
	float remainingDuration = duration;
	static inline const std::string spritePath = "../assets/sprites/projectiles/default.png";
};

