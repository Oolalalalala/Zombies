#pragma once
#include "Enemy.h"
class ShadowDragon :public Enemy
{
public:
	ShadowDragon() = default;
	int _type = 4;
	ShadowDragon(Ref<Scene> scene,int level);
};