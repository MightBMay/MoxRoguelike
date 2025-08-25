#pragma once
#include "pch.h"
#include "Enemy.h"
#include "EnemyManager.h"


namespace Enemies {
	class ScissorEnemy : public Enemy {
	public:
		ScissorEnemy(int level) : Enemy(level, "scissor enemy") {}

	};
}