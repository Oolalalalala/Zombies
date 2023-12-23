#pragma once
#include "Tower.h"
class BallistaTower :public Tower
{
public:
	BallistaTower() = default;
	BallistaTower(Ref<Scene> scene);

protected:
	void changeModel(Ref<Scene> scene);
};