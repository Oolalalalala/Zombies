#include "Loading.h"


Loading::Loading()
{
	m_Scene = CreateRef<Scene>("Loading");

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
	}

	{
		m_LoadingText = m_Scene->CreateEntity("loading text");
		auto& UI = m_LoadingText.AddComponent<UIElementComponent>();
		UI.Anchor = { 0, -1 };
		UI.Pivot = { 0, -1 };
		UI.Offset = { 0, 100 };
		auto& text = m_LoadingText.AddComponent<TextRendererComponent>();
		text.Font = m_Font;
		text.FontSize = 50.0f;
		text.Text = "Loading ...";
		text.Color = glm::vec4(1, 1, 1, 1);
		text.Size = {200, 100};
	}
}

Loading::~Loading()
{
	AssetManager::UnloadFont(m_Font);
	AssetManager::UnloadTexture(m_LoadingTexture);
}

void Loading::OnUpdate(float dt)
{
	auto& sprite = m_BackgroundImage.GetComponent<SpriteRendererComponent>();
	glm::vec2 size1 = sprite.Size / sprite.Size.y * IO::GetWindowSize().y;
	glm::vec2 size2 = sprite.Size / sprite.Size.x * IO::GetWindowSize().x;
	sprite.Size = size1.x > size2.x ? size1 : size2;

	m_Scene->OnUpdate(dt);
}
