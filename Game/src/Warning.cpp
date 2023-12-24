#include "Warning.h"
Warning::Warning(Ref<Scene> m_Scene, int code)
{
	
	_entity = m_Scene->CreateEntity("Text");

	auto& transform = _entity.GetComponent<TransformComponent>();
	transform.Scale *= 2.f;


	auto& UI = _entity.AddComponent<UIElementComponent>(); //加UIElementComponent讓物體變成2D的，物體位置由下面三行決定，Transform.Position沒用了
	UI.Anchor = glm::vec2(-1.0f, -1.0f); //決定響應式視窗的基準點 範圍x,y=[-1, 1]
	UI.Pivot = glm::vec2(-1.0f, -1.0f); //決定文字框的基準點
	UI.Offset = glm::vec2(0.0f, 0.0f); //決定兩基準點的距離

	auto& textRenderer = _entity.AddComponent<TextRendererComponent>();
	textRenderer.Text = ""; //顯示文字內容
	static AssetHandle font = AssetManager::LoadFont("Fonts/OpenSans-Regular.ttf"); //load字體
	textRenderer.Font = font;
	textRenderer.FontSize = 20.0f; //字體大小
	textRenderer.Size = glm::vec2(200.0f, 100.0f); //文字框大小
	textRenderer.OutlineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); //字體顏色RGBA A是透明度
	textRenderer.OutlineThickness = 0.16f; // In the range [0, 0.3f] (not sure) //邊框粗細

	switch (code) {
		case 1: //升級達到上限
		{
			_type = 1; //警告
			_showtime = 400;
			UI.Anchor = glm::vec2(0.f, 0.7f);
			UI.Pivot = glm::vec2(0.f, 0.f);

			textRenderer.Size = glm::vec2(250.0f, 100.0f);//not in the middle
			textRenderer.Text = "Alr Lvl 4!";

			break;
		}
		case 2: //蓋東西
		{
			_type = 1; // 警告
			_showtime = 400;
			UI.Anchor = glm::vec2(0.f, 0.7f);
			UI.Pivot = glm::vec2(0.f, 0.f);

			textRenderer.Size = glm::vec2(100.0f, 100.0f);//not in the middle
			textRenderer.Text = "Cannot Build!";

			break;
		}
		case 3: //已經有東西在蓋了
		{
			_type = 2; // 通知
			_showtime = 400;
			UI.Anchor = glm::vec2(0.f, 0.7f);
			UI.Pivot = glm::vec2(0.f, 0.f);

			textRenderer.Size = glm::vec2(200.0f, 100.0f);//not in the middle
			textRenderer.Text = "Finish Current Build First!";

			break;
		}
		default:
		{
			_type = 0;
			_showtime = 1;
			textRenderer.OutlineThickness = 0.f;
			break;
		}
	}

	switch (_type) {
		case 1: //紅色警告
		{
			textRenderer.OutlineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
		case 2: //藍色通知
		{
			textRenderer.OutlineColor = glm::vec4(0.0f, 0.0f, 0.75f, 1.0f);
			break;
		}
	}


	_code = code;
}

Warning::~Warning() 
{

}

void Warning::Destroy(Ref<Scene> scene)
{
	if (_entity) {
		scene->DestroyEntity(_entity);
		_entity = {};
	}
}

void Warning::update(Ref<Scene> scene, float dt)
{
	if (_phase < _showtime)
	{
		_phase+=1;
		auto& textRenderer = _entity.GetComponent<TextRendererComponent>();
		textRenderer.Color.a = double(_showtime - _phase) / _showtime;
	}
	else
		Destroy(scene);
}

int Warning::getShowtime()
{
	return _showtime;
}
int Warning::getphase()
{
	return _phase;
}
void Warning::setPhase(int phi)
{
	_phase = phi;
}

int Warning::getCode()
{
	return _code;
}
int Warning::getType()
{
	return _type;
}