#pragma once
#include "Enemy.h"
class MonsterSkull :public Enemy
{
public:
	MonsterSkull() = default;
	MonsterSkull(Ref<Scene> scene,int level);
};