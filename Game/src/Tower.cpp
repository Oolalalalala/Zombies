#include "Tower.h"

Tower::Tower(Ref<Scene> scene, double hp, double damage, Asset modelAsset) {

	_entity = AssetLibrary::GetModel(modelAsset);
	_entity.GetComponent<MeshRendererComponent>().Enabled = true;

	_hp = hp;
	_damage = damage;
}
Tower::~Tower() {
}

void Tower::Destroy(Ref<Scene> scene)
{
	scene->DestroyEntity(_entity);
}

void Tower::setPosition(glm::vec3 position)
{
	auto& transform = _entity.GetComponent<TransformComponent>();
	transform.Position = position;
}
void Tower::setRotation(glm::quat rotation)
{
	auto& transform = _entity.GetComponent<TransformComponent>();
	transform.Rotation = rotation;
}
void Tower::takeDamage(double D)
{
	_hp -= D;
}
void Tower::setlevel(int l, Ref<Scene> scene)
{
	_level = l;
	changeModel(scene);
	_entity.GetComponent<MeshRendererComponent>().Enabled = true;
}




double Tower::getDamage()
{
	return _damage;
}
double Tower::getHp()
{
	return _hp;
}
double Tower::getLevel()
{
	return _level;
}