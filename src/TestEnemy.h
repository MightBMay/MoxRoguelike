#pragma once
#include "Enemy.h"

class TestEnemy : public Enemy {

public:
	TestEnemy(int level) :Enemy(level, "test enemy") {

	}

};