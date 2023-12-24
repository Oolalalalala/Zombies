#include "Map.h"
#include <cstdlib>
#include <ctime>
Map::Map(Ref<Scene> scene, int size) {
	for (int i = 0;i < size;i++) {
		for (int j = 0;j < size;j++) {
			mapinfo[i][j] = 0;
			pathinfo[i][j] = glm::ivec2(-1, -1);
			if (i == 25 && j == 25)mapinfo[i][j] = 100;
			if (i == 0 || i == 50 || j == 0 || j == 50)mapinfo[i][j] = 3;
			_floor[i][j] = CreateFloor(scene, i, j);
		}
	}
}

Entity Map::CreateFloor(Ref<Scene> scene, int i, int j) {
	Entity quad = scene->CreateEntity("Green Floor");

	if (false) {
		auto& spriteRenderer = quad.AddComponent<SpriteRendererComponent>(); // AddComponent 賦予性質
		spriteRenderer.Size = glm::vec2(94.f, 94.f);
		spriteRenderer.Texture = AssetManager::LoadTexture("Texture/floor2.png");
	}
	else {
		auto& spriteRenderer = quad.AddComponent<SpriteRendererComponent>(); // AddComponent 賦予性質
		spriteRenderer.Size = glm::vec2(94.f, 94.f);

		spriteRenderer.Color = DecideColor(mapinfo[i][j]);
	}

	auto& transform = quad.GetComponent<TransformComponent>();
	transform.Position = { i * 96, 0, j * 96 }; // 不會影響物體位置(被UIElementComponent覆蓋)
	transform.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	transform.Rotation = glm::rotate(transform.Rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	return quad;
}

void Map::ChangeColor(Ref<Scene> scene, int i, int j, int status) {
	if(status!=2)mapinfo[i][j] = status;
	auto& spriteRenderer = _floor[i][j].GetComponent<SpriteRendererComponent>(); // AddComponent 賦予性質z
	spriteRenderer.Color = DecideColor(status);
}

glm::ivec2 Map::FindFloor(glm::vec3 lookat, glm::vec3 pos) {

	if (lookat.y > -0.05)
		return glm::ivec2(-1);

	glm::ivec2 grid = Pos2Map(pos - lookat / lookat.y * pos.y);
	if (grid.x >= 51 || grid.y >= 51 || grid.x < 0 || grid.y < 0)
		return glm::ivec2(-1);

	return grid;
}

glm::ivec2 Map::Pos2Map(glm::vec3 pos)
{
	pos += glm::vec3(48.f, 0.f, 48.f);
	glm::ivec2 ans(0, 0);
	ans.x = int(pos.x) / 96;
	ans.y = int(pos.z) / 96;
	return ans;
}

glm::vec4 Map::DecideColor(int status) {
	glm::vec4 ans(0.f, 0.f, 0.f, 1.f);
	switch (status) {
		case 1: {//tower : blue
			ans = glm::vec4(0.f, 0.f, 1.f, 1.f);
			break;
		}
		case 2: {//lookat : orange
			ans = glm::vec4(1.f, 0.4f, 0.1f, 1.f);
			break;
		}
		case 3: {//mob spawn : brown
			ans = glm::vec4(0.5f, 0.16f, 0.16f, 1.f);
			break;
		}
		case 4: {//path : red
			ans = glm::vec4(1.f, 0.f, 0.f, 1.f);
			break;
		}
		case 100: {//core : yellow
			ans = glm::vec4(1.f, 1.f, 0.f, 1.f);
			break;
		}
		default: {//space : green
			ans = glm::vec4(0.f, 1.f, 0.f, 1.f);
			break;
		}
	}
	return ans;
}

glm::ivec2 Map::DecidePath(Ref<Scene> scene) {
	srand(time(NULL));
	int test = ((rand()%200)*67) % 200;
	//start is a random point on edge, end is always on core
	glm::ivec2 start(0, 0), end(25, 25);
	if (test < 50)       start = glm::ivec2(0, test);
	else if (test < 100) start = glm::ivec2(test - 50, 50);
	else if (test < 150) start = glm::ivec2(test - 99, 0);
	else                 start = glm::ivec2(50, test - 149);
	glm::ivec2 ans = start;

	while (start.x != end.x || start.y != end.y) {
		mapinfo[start.x][start.y] = 4;
		ChangeColor(scene, start.x, start.y, 4);
		int delta_x = start.x - end.x;
		int delta_z = start.y - end.y;
		int ham_dis = abs(delta_x) + abs(delta_z);
		int togo = rand() % ham_dis;
		if (togo < abs(delta_x)) {
			int d = (delta_x > 0) ? 1 : -1;
			pathinfo[start.x][start.y] = glm::ivec2(start.x - d, start.y);
			start.x -= d;
		}
		else {
			int d = (delta_z > 0) ? 1 : -1;
			pathinfo[start.x][start.y] = glm::ivec2(start.x, start.y - d);
			start.y -= d;
		}
	}
	return ans;
}