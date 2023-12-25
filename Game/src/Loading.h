#pragma once

#include "Engine.h"

class Loading
{
public:
	Loading();
	~Loading();

	void OnUpdate(float dt);
private:
	Ref<Scene> m_Scene;

	Entity m_BackgroundImage;
	Entity m_LoadingText;

	AssetHandle m_Font;
	AssetHandle m_LoadingTexture;
};