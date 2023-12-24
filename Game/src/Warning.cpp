#include "Warning.h"
Warning::Warning(Ref<Scene> m_Scene, int code)
{
	
	_entity = m_Scene->CreateEntity("Text");

	auto& transform = _entity.GetComponent<TransformComponent>();
	transform.Scale *= 2.f;


	auto& UI = _entity.AddComponent<UIElementComponent>(); //�[UIElementComponent�������ܦ�2D���A�����m�ѤU���T��M�w�ATransform.Position�S�ΤF
	UI.Anchor = glm::vec2(-1.0f, -1.0f); //�M�w�T��������������I �d��x,y=[-1, 1]
	UI.Pivot = glm::vec2(-1.0f, -1.0f); //�M�w��r�ت�����I
	UI.Offset = glm::vec2(0.0f, 0.0f); //�M�w�����I���Z��

	auto& textRenderer = _entity.AddComponent<TextRendererComponent>();
	textRenderer.Text = ""; //��ܤ�r���e
	static AssetHandle font = AssetManager::LoadFont("Fonts/OpenSans-Regular.ttf"); //load�r��
	textRenderer.Font = font;
	textRenderer.FontSize = 20.0f; //�r��j�p
	textRenderer.Size = glm::vec2(200.0f, 100.0f); //��r�ؤj�p
	textRenderer.OutlineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); //�r���C��RGBA A�O�z����
	textRenderer.OutlineThickness = 0.16f; // In the range [0, 0.3f] (not sure) //��زʲ�

	switch (code) {
		case 1: //�ɯŹF��W��
		{
			_type = 1; //ĵ�i
			_showtime = 400;
			UI.Anchor = glm::vec2(0.f, 0.7f);
			UI.Pivot = glm::vec2(0.f, 0.f);

			textRenderer.Size = glm::vec2(250.0f, 100.0f);//not in the middle
			textRenderer.Text = "Alr Lvl 4!";

			break;
		}
		case 2: //�\�F��
		{
			_type = 1; // ĵ�i
			_showtime = 400;
			UI.Anchor = glm::vec2(0.f, 0.7f);
			UI.Pivot = glm::vec2(0.f, 0.f);

			textRenderer.Size = glm::vec2(100.0f, 100.0f);//not in the middle
			textRenderer.Text = "Cannot Build!";

			break;
		}
		case 3: //�w�g���F��b�\�F
		{
			_type = 2; // �q��
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
		case 1: //����ĵ�i
		{
			textRenderer.OutlineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
		case 2: //�Ŧ�q��
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