#pragma once

#include "Engine.h"
#include "AssetLibrary.h"
class Enemy
{
public:
	Enemy(Ref<Scene> scene, int level, Asset modelAsset);
	~Enemy();
	glm::vec3 getPosition();
	glm::quat getRotation();
	void setPosition(glm::vec3 position);
	void Destroy(Ref<Scene> scene);
	void setRotation(glm::quat rotation);
	void update(float dt);
	void takeDamage(double);
	double getDamage();
	double gethp();
	int getLevel();
	int	_type;
	
protected:
	double _hp, _damage;
	int _level;
	Entity _entity;
};