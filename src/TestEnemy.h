#pragma once
#include "Enemy.h"

namespace Enemies {
	class TestEnemy : public Enemy {

	public:
		TestEnemy(int level) :Enemy(level, "test enemy") {

		}

	};
}