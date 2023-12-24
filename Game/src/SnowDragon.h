#pragma once
#include "Enemy.h"
class SnowDragon :public Enemy
{
public:
	SnowDragon() = default;
	SnowDragon(Ref<Scene> scene);
	int _type = 5;
};