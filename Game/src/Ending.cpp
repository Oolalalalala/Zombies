#include "Ending.h"

Ending::Ending()
{
	m_Scene = CreateRef<Scene>("Ending");
	
	AssetManager::SetAssetDirectory("asset");
	m_Font = AssetManager::LoadFont("Fonts/OpenSans-Bold.ttf");
	m_LoadingTexture = AssetManager::LoadTexture("Texture/gameplay.png");
	
	{
		m_BackgroundImage = m_Scene->CreateEntity("background");
		auto& UI = m_BackgroundImage.AddComponent<UIElementComponent>();
		auto& sprite = m_BackgroundImage.AddComponent<SpriteRendererComponent>();
		sprite.Size = { 1920, 1080 };
		sprite.Texture = m_LoadingTexture;
		sprite.Color = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);

		m_BackgroundImage.AddComponent<ButtonComponent>();
	}

	{
		m_ScoreText = m_Scene->CreateEntity("score display");
		auto& UI = m_ScoreText.AddComponent<UIElementComponent>();
		UI.Anchor = { 0, -1 };
		UI.Pivot = { 0, -1 };
		UI.Offset = { 0, 100 };
		auto& text = m_ScoreText.AddComponent<TextRendererComponent>();
		text.Font = m_Font;
		text.FontSize = 50.0f;
		text.Text = "Score: ???";
		text.Color = glm::vec4(1, 1, 1, 1);
		text.Size = {250, 100};
	}

	{
		m_ContinueText = m_Scene->CreateEntity("score display");
		auto& UI = m_ContinueText.AddComponent<UIElementComponent>();
		UI.Anchor = { 0, -1 };
		UI.Pivot = { 0, -1 };
		UI.Offset = { 0, 30 };
		auto& text = m_ContinueText.AddComponent<TextRendererComponent>();
		text.Font = m_Font;
		text.FontSize = 20.0f;
		text.Text = "Click To Continue";
		text.Color = glm::vec4(1, 1, 1, 1);
		text.Size = {160, 100};
	}
}

Ending::~Ending()
{
	AssetManager::UnloadFont(m_Font);
	AssetManager::UnloadTexture(m_LoadingTexture);
}

void Ending::OnUpdate(float dt)
{
	auto& sprite = m_BackgroundImage.GetComponent<SpriteRendererComponent>();
	glm::vec2 size1 = sprite.Size / sprite.Size.y * IO::GetWindowSize().y;
	glm::vec2 size2 = sprite.Size / sprite.Size.x * IO::GetWindowSize().x;
	sprite.Size = size1.x > size2.x ? size1 : size2;

	m_Scene->OnUpdate(dt);
}

void Ending::SetRestartCallback(std::function<void()> callback)
{
	m_BackgroundImage.GetComponent<ButtonComponent>().ButtonDownCallback = callback;
}

void Ending::SetScore(int score)
{
	m_Score = score;
	auto& text = m_ScoreText.GetComponent<TextRendererComponent>();
	std::string str = std::to_string(score);
	text.Text = "Score:" + str;
	for (int i = 0; i < 4 - str.length(); i++)
		text.Text = " " + text.Text;
}
