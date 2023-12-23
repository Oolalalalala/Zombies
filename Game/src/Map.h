#pragma once

#include "Engine.h"

class Map
{
public:
	int mapinfo[51][51];
	Map(Ref<Scene> scene, int size = 51);
	Entity CreateFloor(Ref<Scene> scene, int i, int j);
	void DestroyFloor(Ref<Scene> scene, int i, int j);
	void UpdateMap(Ref<Scene> scene, int x, int y, int status);
	void ChangeColor(Ref<Scene> scene, int i, int j, int status);
	void DecidePath(Ref<Scene> scene);

	glm::ivec2 FindFloor(glm::vec3 lookat, glm::vec3 pos);
	glm::vec4 DecideColor(int status);

protected:
	Entity _floor[51][51];
	int _code, _type, _showtime, _phase = 0;
};

