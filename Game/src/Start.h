#pragma once
#include "Engine.h"

class Start 
{
public:
	Start();
	~Start();
	void OnUpdate(float dt);
	void SetStartCallback(std::function<void()> callback);
	

private:
	Ref<Scene> s_Scene;
	Entity s_Camera;
	Entity s_PressToPlay;
	Entity s_ChenChengPo;
	AssetHandle s_PTP;
	AssetHandle s_CCP;
private:
	Entity CreateCamera();
	void SetPosition(int,int);
};