#pragma once

#include "Engine.h"

class Map
{
public:
	int mapinfo[51][51];
	glm::ivec2 pathinfo[51][51];

	Map(Ref<Scene> scene, int size = 51);
	Entity CreateFloor(Ref<Scene> scene, int i, int j);
	void ChangeColor(Ref<Scene> scene, int i, int j, int status);

	glm::ivec2 DecidePath(Ref<Scene> scene);
	glm::ivec2 FindFloor(glm::vec3 lookat, glm::vec3 pos);
	glm::ivec2 Pos2Map(glm::vec3 pos);
	glm::vec4 DecideColor(int status);

protected:
	Entity _floor[51][51];
};

