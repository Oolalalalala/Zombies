#pragma once

#include "Engine.h"


class Ending
{
public:
	Ending();
	~Ending();

	void OnUpdate(float dt);
	void SetRestartCallback(std::function<void()> callback);
	void SetScore(int score);

private:
	Ref<Scene> m_Scene;
	
	AssetHandle m_Font;
	AssetHandle m_LoadingTexture;

	Entity m_BackgroundImage;
	Entity m_ScoreText;
	Entity m_ContinueText;

	int m_Score = 0;
};