#pragma once

#include "Projectile.h"
class EnemyManager;
class GameObject;


class Reap_Projectile : public Projectile {

public:
	Reap_Projectile(sf::Vector2f direction, ProjectileStats stats, const sf::IntRect& textureRect);
	void update(float deltaTime) override final;
	void init() override final;



protected:

private:
	const float duration = 7.5f;
	float remainingDuration = duration;
	static inline const std::string spritePath = "../assets/sprites/projectiles/default.png";
};

