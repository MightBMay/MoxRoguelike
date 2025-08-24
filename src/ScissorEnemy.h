#pragma once
#include "pch.h"
#include "Enemy.h"
#include "EnemyManager.h"



class ScissorEnemy : public Enemy{
public:
	ScissorEnemy(int level): Enemy(level,"scissor enemy") {}

};