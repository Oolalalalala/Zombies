#include <Enemy.h>

Enemy::Enemy(Ref<Scene> scene, double hp, double damage,const std::string& pp) {
	_entity = Model::Load(scene, pp); // ¸ü¤J¼Ò«¬
	_hp = hp;
	_damage = damage;
}
Enemy::~Enemy(){
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