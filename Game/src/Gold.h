#pragma once
#include "Engine.h"
#include "AssetLibrary.h"
class Gold
{
public:
	int gold=0;
	Entity block;
public:
	Gold(Ref<Scene> scene);
	~Gold();
	void UpdateGold(int);
	void UpdateBlock();
};