#pragma once
#include "Enemy.h"
class BabyGrogu :public Enemy
{
public:
	BabyGrogu() = default;
	BabyGrogu(Ref<Scene> scene);
};