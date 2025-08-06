#pragma once
#include "Enemy.h"

class TestEnemy : public Enemy {

public:
	TestEnemy(int level) :Enemy(level) {

	}

protected:
	virtual int hpPerLevel() const override { return 3; }
	virtual int damagePerLevel() const override { return 6; }
	virtual float attackSpeed() const override { return 5; }
	virtual const float attackSize() const override { return 6; }
	virtual float speed() const override { return 175; }
	virtual float size() const override { return 86; }
};