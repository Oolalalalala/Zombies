#include "Gold.h"
Gold::Gold(Ref<Scene> scene)
{
	block = scene->CreateEntity("Text");

	auto& transform = block.GetComponent<TransformComponent>();
	transform.Scale *= 2.f;


	auto& UI = block.AddComponent<UIElementComponent>(); //加UIElementComponent讓物體變成2D的，物體位置由下面三行決定，Transform.Position沒用了
	UI.Anchor = glm::vec2(+1.0f, +1.0f);
	UI.Pivot = glm::vec2(+1.0f, +1.0f);
	UI.Offset = glm::vec2(-10.0f, -10.0f);

	auto& textRenderer = block.AddComponent<TextRendererComponent>();
	textRenderer.Text = "GOLD=0";
	textRenderer.Font = AssetLibrary::Get(Asset::OpenSansFont);
	textRenderer.FontSize = 20.0f;
	textRenderer.Size = glm::vec2(200.0f, 100.0f);
	textRenderer.OutlineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	textRenderer.OutlineThickness = 0.16f; // In the range [0, 0.3f] (not sure)
}
Gold::~Gold()
{

}
void Gold::UpdateGold(int g)
{
	gold += g;
	UpdateBlock();
}
void Gold::UpdateBlock()
{
	auto& text = block.GetComponent<TextRendererComponent>();
	text.Text = "GOLD:" + std::to_string(gold);
}