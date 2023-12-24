#pragma once
#include "Enemy.h"
class ShadowDragon :public Enemy
{
public:
	ShadowDragon() = default;
	ShadowDragon(Ref<Scene> scene);
	int _type = 4;
};