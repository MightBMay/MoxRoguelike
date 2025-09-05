#pragma once
#include "Weapon.h"
#include "Animation.h"

namespace playerClasses {
	class Ninja;
}

class Ninja_E : public AbilityBase {
private:
	
	playerClasses::Ninja* player = nullptr;
	float dashDuration = 0;
	float remainingDashDuration = 0;
	float invulnDuration = 0;
	float remeainingInvulnDuration = 0;
	bool dashFinished = true;
	bool invulnFinished = true;

public:

	Ninja_E(playerClasses::Ninja* player) : player(player), AbilityBase("Ninja E") {}

	void update(float deltaTime)override;
	void Fire() override;

	const json& LoadInfoFromJson() override;

};