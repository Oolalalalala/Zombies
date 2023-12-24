#pragma once
#include "Enemy.h"
class BlueDragon :public Enemy
{
public:
	BlueDragon() = default;
	BlueDragon(Ref<Scene> scene);
	int _type = 1;
};