#pragma once

#include "Engine.h"
#include "AssetLibrary.h"
class Enemy
{
public:
	Enemy(Ref<Scene> scene, double hp, double damage, Asset modelAsset);
	~Enemy();
	glm::vec3 getPosition();
	glm::quat getRotation();
	void setPosition(glm::vec3 position);
	void Destroy(Ref<Scene> scene);
	void setRotation(glm::quat rotation);
	void setVelocity(glm::vec3 v);
	void update(float dt);
	void takeDamage(double);
	double getDamage();
	double gethp();
	int	_type;
	
protected:
	double _hp, _damage;
	Entity _entity;
	glm::vec3 _velocity;
};