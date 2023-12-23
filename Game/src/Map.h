#pragma once

#include "Engine.h"

class Map
{
public:
	int mapinfo[51][51];
	Entity CreateFloor(Ref<Scene> scene, int i, int j);
	void DestroyFloor(Ref<Scene> scene, int i, int j);
	Map(Ref<Scene> scene, int size = 51);
	void UpdateMap(Ref<Scene> scene, int x, int y, int status);
	glm::ivec2 FindFloor(glm::vec3 lookat, glm::vec3 pos);
	void ChangeColor(Ref<Scene> scene, int i, int j, int status);

protected:
	Entity _floor[51][51];
	int _code, _type, _showtime, _phase = 0;
};

