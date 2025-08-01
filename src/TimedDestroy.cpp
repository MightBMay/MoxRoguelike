#include "TimedDestroy.h"
#include "EnemyManager.h"
void TimedDestroy::update(float deltaTime) {
	timeRemaining -= deltaTime;
	if (timeRemaining <= 0)
		EnemyManager::removeHitboxVisual(parent);
}