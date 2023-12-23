#pragma once

#include "Engine.h"
#include "AssetLibrary.h"

class Tower
{
public:
	Tower(Ref<Scene> scene, double hp, double damage, Asset modelAsset);
	~Tower();

	void setPosition(glm::vec3 position);
	void Destroy(Ref<Scene> scene);
	void setRotation(glm::quat rotation);
	void update(float dt);
	void takeDamage(double D);
	void setlevel(int l, Ref<Scene> scene);

	double getDamage();
	double getHp();
	double getLevel();

	

protected:
	double _hp, _damage;
	Entity _entity;
	int _level=1;
	virtual void changeModel(Ref<Scene> scene) = 0;

};