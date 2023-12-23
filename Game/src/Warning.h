#pragma once

#include "Engine.h"

class Warning
{
public:
	Warning(Ref<Scene> scene, int code);
	~Warning();

	//void setPosition(glm::vec3 position);
	void Destroy(Ref<Scene> scene);
	//void setRotation(glm::quat rotation);
	void update(Ref<Scene> scene, float dt);
	//void takeDamage(double D);
	//void setlevel(int l, Ref<Scene> scene);
	int getShowtime();
	int getphase();
	void setPhase(int phi);
	int getCode();
	int getType();
	


protected:
	Entity _entity;
	int _code, _type, _showtime, _phase=0;

};

