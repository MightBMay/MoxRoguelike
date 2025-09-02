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

		EnemyManager::getInRange(pullPosition, aoeSize, enemiesToPull);
		for (auto& enemy : enemiesToPull) {
			enemy.lock()->getDerivativesOfComponent<Enemies::Enemy>()->takeDamage(damage);
		}

		attackTimer = attackSpeed;



	}

	virtual void update(float deltaTime) override final {
		if (Input::GetActionDown("ability3") && attackTimer <= 0) {
			Fire();
		}

		if (remainingPullDuration > 0 && !enemiesToPull.empty()) {
			for (auto& enemy : enemiesToPull) {
				auto enemyS = enemy.lock();
				sf::Vector2f direction = pullPosition - enemyS->getPosition();
				enemyS->move(direction * pullSpeed * deltaTime);
			}
			// decrement remainintPullDuration and compare.
			if ((remainingPullDuration -= deltaTime) <= 0) {
				enemiesToPull.clear(); // clear enemies to pull, and reset pull position.
				pullPosition = {};
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