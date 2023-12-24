#include "H_PBar.h"
H_PBar::H_PBar(Ref<Scene> scene)
{
	OriginBar = scene->CreateEntity("origin quad");
	OriginBar.AddComponent<SpriteRendererComponent>(glm::vec2{ hp * 0.01, 10.f }, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f }); // AddComponent 賦予性質

	auto& transform1 = OriginBar.GetComponent<TransformComponent>();	//GetComponent 取得性質
	transform1.Position = { 2400, 100, 2400 };

	PresentBar = scene->CreateEntity("present quad");
	PresentBar.AddComponent<SpriteRendererComponent>(glm::vec2{ hp*0.01, 10.f }, glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }); // AddComponent 賦予性質

	auto& transform2 = PresentBar.GetComponent<TransformComponent>();	//GetComponent 取得性質
	transform2.Position = { 2400, 100, 2400 };
}
H_PBar::~H_PBar()
{

}

void H_PBar::UpdateH_PBar(glm::vec3 position)
{
	PresentBar.GetComponent<SpriteRendererComponent>().Size=glm::vec2{ hp * 0.01, 10.f }; // AddComponent 賦予性質
	OriginBar.GetComponent<SpriteRendererComponent>().Size = glm::vec2{ 100-(hp * 0.01), 10.f };

	auto& transform1 = OriginBar.GetComponent<TransformComponent>();	//GetComponent 取得性質
	auto& transform2 = PresentBar.GetComponent<TransformComponent>();	//GetComponent 取得性質

	glm::vec3 a = position - glm::vec3(2400,100,2400);
	glm::vec3 dir = glm::normalize(a);
	transform2.Rotation = glm::quat(glm::vec3(0, 1.0f, 0), dir);
	transform1.Rotation = glm::quat(glm::vec3(0, 1.0f, 0), dir);

	glm::vec3 left = glm::cross(dir, glm::vec3(0, 1, 0.0f));
	glm::vec3 d = glm::normalize(left);
	transform2.Position = glm::vec3(2400, 100, 2400) + (10000 - hp )* 0.01f *d/2.f;
	transform1.Position = glm::vec3(2400, 100, 2400) - hp * 0.01f *d/2.f;
	
}

void H_PBar::TakeDamage(float damage)
{
	hp -= damage;
}

float H_PBar::GetHp()
{
	return hp;
}
