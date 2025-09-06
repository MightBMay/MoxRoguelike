#include "pch.h"
#include "Ninja_E.h"
#include "Ninja.h"


void Ninja_E::update(float deltaTime) {
	attackTimer -= deltaTime;

	if (Input::GetActionDown("ability2") && attackTimer <= 0)
		Fire();


	remainingDashDuration -= deltaTime;
	remeainingInvulnDuration -= deltaTime;

	// since the only way the player class can be deleted is by fully resetting the game ( thus also deleting 
	// this weapon) we don't need to nullptr check the player ptr.
	if (!dashFinished && remainingDashDuration <= 0) {
		dashFinished = true;
		player->StopDash();
		attackTimer = playerStats->AttackSpeed(attackSpeed);

	}
	if (!invulnFinished && remeainingInvulnDuration <= 0) {
		invulnFinished = true;
		player->SetVulnrable(true);
	}


	cooldownTickEvent.invoke(attackTimer);
}

void Ninja_E::Fire() {
	player->UpdateMoveDirection();
	if (player->direction.lengthSquared() == 0) return;


	remainingDashDuration = dashDuration;
	remeainingInvulnDuration = invulnDuration;
	player->StartDash(speed);
	player->SetVulnrable(false);
	dashFinished = false;
	invulnFinished = false;

	

}

const json& Ninja_E::LoadInfoFromJson(){
	const json& data = AbilityBase::LoadInfoFromJson();

	if (data.contains("dash duration"))
		dashDuration = data["dash duration"].get<float>();
	else
		std::cerr << "\n dash duration not found for Ninja E";

	if (data.contains("invuln duration"))
		invulnDuration = data["invuln duration"].get<float>();
	else
		std::cerr << "\n invuln duration not found for Ninja E";


	return data;
}