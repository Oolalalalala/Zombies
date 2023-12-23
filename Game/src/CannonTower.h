#pragma once
#include "Tower.h"
class CannonTower :public Tower
{
public:
	CannonTower() = default;
	CannonTower(Ref<Scene> scene);

protected:
	void changeModel(Ref<Scene> scene);
};