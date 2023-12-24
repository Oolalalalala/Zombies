#include <Enemy.h>

Enemy::Enemy(Ref<Scene> scene, double hp, double damage, Asset modelAsset) {
	_entity = AssetLibrary::GetModel(modelAsset);
	if (_entity.HasComponent<MeshRendererComponent>())
		_entity.GetComponent<MeshRendererComponent>().Enabled = true;
	if (_entity.HasComponent<SkinnedMeshRendererComponent>())
		_entity.GetComponent<SkinnedMeshRendererComponent>().Enabled = true;

	_hp = hp;
	_damage = damage;
	_velocity = glm::vec3(0,0,0);
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
void Enemy::setVelocity(glm::vec3 v)
{
	_velocity = v;
}
void Enemy::update(float dt)
{
	auto& transform = _entity.GetComponent<TransformComponent>();
	transform.Position += _velocity*dt;
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

glm::vec3 Enemy::getPos()
{
	auto& transform = _entity.GetComponent<TransformComponent>();
	return transform.Position;
}

glm::quat Enemy::getRotation()
{
	auto& transform = _entity.GetComponent<TransformComponent>();
	return transform.Rotation;
}
