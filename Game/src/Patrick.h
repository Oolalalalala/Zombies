#pragma once
#include "Enemy.h"
class Patrick :public Enemy
{
public:
	Patrick() = default;
	Patrick(Ref<Scene> scene,int level);
	int _type = 8;
};