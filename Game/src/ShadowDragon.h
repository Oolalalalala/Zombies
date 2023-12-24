#pragma once
#include "Enemy.h"
class ShadowDragon :public Enemy
{
public:
	ShadowDragon() = default;
	ShadowDragon(Ref<Scene> scene,int level);
};