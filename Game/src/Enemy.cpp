#include <Enemy.h>

Enemy::Enemy(Ref<Scene> scene, int level, Asset modelAsset) {
	_entity = AssetLibrary::GetModel(modelAsset);
	if (_entity.HasComponent<MeshRendererComponent>())
		_entity.GetComponent<MeshRendererComponent>().Enabled = true;
	if (_entity.HasComponent<SkinnedMeshRendererComponent>())
		_entity.GetComponent<SkinnedMeshRendererComponent>().Enabled = true;

	_hp = 100* level;
	_damage = 100 * level;
	_level = level;
	_entity.GetComponent<TransformComponent>().Scale *=_level*1.0f;

}
Enemy::~Enemy(){
}

glm::vec3 Enemy::getPosition()
{
	return _entity.GetComponent<TransformComponent>().Position;
}

glm::quat Enemy::getRotation()
{
	return _entity.GetComponent<TransformComponent>().Rotation;
}

void Enemy::Destroy(Ref<Scene> scene)
{
	scene->DestroyEntity(_entity);
}




void Enemy::setPosition(glm::vec3 position)
{
	auto& transform = _entity.GetComponent<TransformComponent>();
	transform.Position = position;
}
void Enemy::setRotation(glm::quat rotation)
{
	auto& transform = _entity.GetComponent<TransformComponent>();
	transform.Rotation= rotation;
}
void Enemy::update(float dt)
{

}





void Enemy::takeDamage(double D)
{
	_hp -= D;
}
double Enemy::getDamage()
{
	return _damage;
}
double Enemy::gethp()
{
	return _hp;
}