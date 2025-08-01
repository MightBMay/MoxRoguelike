#pragma once
#include "Component.h"
#include "GameObject.h"

class TimedDestroy : public Component {
public:
	TimedDestroy(float duration) : timeRemaining(duration) {}
	void update(float deltaTime) override;
	void Destroy() override {}
	virtual void ProcessEvent(const std::optional<sf::Event>& event) override {};

private:
	float timeRemaining;

};