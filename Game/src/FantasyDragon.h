#pragma once
#include "Enemy.h"
class FantasyDragon :public Enemy
{
public:
	FantasyDragon() = default;
	FantasyDragon(Ref<Scene> scene);
	int _type = 2;
};