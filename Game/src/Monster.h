#pragma once
#include "Enemy.h"
class Monster :public Enemy
{
public:
	Monster() = default;
	Monster(Ref<Scene> scene,int level);
};