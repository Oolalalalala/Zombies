#pragma once
#include "Engine.h"
class H_PBar
{
public:
	H_PBar(Ref<Scene> scene);
	~H_PBar();
	void UpdateH_PBar(glm::vec3);
	void TakeDamage(float);
	float GetHp();
private:
	float hp = 10000.0;
	Entity OriginBar;
	Entity PresentBar;
};