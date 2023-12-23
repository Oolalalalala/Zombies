#pragma once
#include "Tower.h"
class PoisonTower :public Tower
{
public:
	PoisonTower() = default;
	PoisonTower(Ref<Scene> scene);

protected:
	void changeModel(Ref<Scene> scene);
};