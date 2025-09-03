#pragma once
#include "weapon.h"
#include "Enemy.h"
#include "EnemyManager.h"

class Papyrmancer_R : public AbilityBase {

private:
	/// <summary>
	/// pre-squared for dist checks.
	/// </summary>
	int aoeSize = 0;
	float pullSpeed = 3.5;
	const float pullDuration = 0.5f;
	float remainingPullDuration = 0;
	static inline sf::Vector2f pullPosition{};
	static inline std::vector<std::weak_ptr<GameObject>> enemiesToPull;

public:
	Papyrmancer_R() :AbilityBase("Papyrmancer R") {
		enemiesToPull.reserve(128);
	}

	virtual void Fire()override final{
		remainingPullDuration = pullDuration;
		pullPosition = Input::mousePos_World;

		EnemyManager::getInRange(pullPosition, aoeSize, enemiesToPull); // get all enemies in range
		std::vector<std::weak_ptr<GameObject>> aliveEnemies; // used to filter any enemies that died in the process.
		int size = enemiesToPull.size(); 

		aliveEnemies.reserve(size);// since its likely enemies will survive the damage.

		for (auto& enemy : enemiesToPull) // iterate enemies in range of ability and damage them
			if (!enemy.lock()->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(damage)) // if damage kills, don't add to aliveEnemies
				aliveEnemies.push_back(enemy);

		// move aliveEnemies to enemiestopull, removing any now dead enemies.
		enemiesToPull = std::move(aliveEnemies);


		attackTimer = attackSpeed;



	}

	virtual void update(float deltaTime) override final {
		if (Input::GetActionDown("ability3") && attackTimer <= 0) {
			Fire();
		}

		if (remainingPullDuration > 0 && !enemiesToPull.empty()) {
			// important we do this before pulling enemies, as if the ability kills the enemy, there isnt a 
			// super simple way to remove it from the ETP vector, so they stay in there until it times out, then it clears.
			if ((remainingPullDuration -= deltaTime) <= 0) { // decrement remainintPullDuration and compare.
				enemiesToPull.clear(); // clear enemies to pull, and reset pull position.
				pullPosition = {};
			}
			for (auto& enemy : enemiesToPull) {
				auto enemyS = enemy.lock();
				if (!enemyS) { continue; }
				sf::Vector2f direction = pullPosition - enemyS->getPosition();
				enemyS->move(direction * pullSpeed * deltaTime);
			}


		}

		attackTimer -= deltaTime;
		cooldownTickEvent.invoke(attackTimer);
		

	}

	virtual const json& LoadInfoFromJson() override {
		const auto& json = AbilityBase::LoadInfoFromJson();

		if (json.contains("aoeSize")) {
			int size = json["aoeSize"];
			aoeSize = size*size;
		}

		

		return json;
	}
};