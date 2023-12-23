#pragma once
#include "Tower.h"
class WizardTower :public Tower
{
public:
	WizardTower() = default;
	WizardTower(Ref<Scene> scene);

protected:
	void changeModel(Ref<Scene> scene);
};