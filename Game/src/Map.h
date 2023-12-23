#pragma once

#include "Engine.h"

class Map
{
public:
	Entity CreateFloor(Ref<Scene> scene, int i, int j);
	void DestroyFloor(Ref<Scene> scene, int i, int j);
	Map(Ref<Scene> scene, int size = 51);
	void UpdateMap(Ref<Scene> scene, int x, int y, int status);

protected:
	int mapinfo[51][51];
	Entity _floor[51][51];
	int _code, _type, _showtime, _phase = 0;
};

