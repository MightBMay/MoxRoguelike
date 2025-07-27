#pragma once
#include "Component.h"
#include "GameObject.h"

class TimedDestroy : public Component {
public:
	TimedDestroy(float duration) : timeRemaining(duration) {}
	void update(float deltaTime) override {
		timeRemaining -= deltaTime;
		if (timeRemaining <= 0)
			GameObjectManager::getInstance().remove(parent);
			//EnemyManager::removeHitboxVisual(parent->shared_from_this());
	}
	void Destroy() override {}
	virtual void ProcessEvent(const std::optional<sf::Event>& event) override {};

private:
	float timeRemaining;

};