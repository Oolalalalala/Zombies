#pragma once
#include "Tower.h"
class ArcherTower :public Tower
{
public:
	ArcherTower() = default;
	ArcherTower(Ref<Scene> scene);

protected:
	void changeModel(Ref<Scene> scene);
};