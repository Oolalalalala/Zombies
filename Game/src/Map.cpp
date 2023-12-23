#include "Map.h"
Map::Map(Ref<Scene> scene, int size) {
	for (int i = 0;i < size;i++) {
		for (int j = 0;j < size;j++) {
			mapinfo[i][j] = 0;
			_floor[i][j] = CreateFloor(scene, i, j);
		}
	}
}

Entity Map::CreateFloor(Ref<Scene> scene, int i, int j) {
	Entity quad = scene->CreateEntity("Green Floor");

	auto& spriteRenderer = quad.AddComponent<SpriteRendererComponent>(); // AddComponent 賦予性質
	spriteRenderer.Size = glm::vec2(94.f, 94.f);
	if (mapinfo[i][j] == 1)//tower
		spriteRenderer.Color = glm::vec4(0.f, 0.f, 1.f, 1.f);
	else if (mapinfo[i][j] == 2)//lookat
		spriteRenderer.Color = glm::vec4(1.f, 0.4f, 0.1f, 1.f);
	else
		spriteRenderer.Color = glm::vec4(0.f, 1.f, 0.f, 1.f);

	auto& transform = quad.GetComponent<TransformComponent>();
	transform.Position = { i * 96, 0, j * 96 }; // 不會影響物體位置(被UIElementComponent覆蓋)
	transform.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	transform.Rotation = glm::rotate(transform.Rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	return quad;
}

void Map::ChangeColor(Ref<Scene> scene, int i, int j, int status) {
	if(status!=2)mapinfo[i][j] = status;
	auto& spriteRenderer = _floor[i][j].GetComponent<SpriteRendererComponent>(); // AddComponent 賦予性質z

	if (status == 1)//tower
		spriteRenderer.Color = glm::vec4(0.f, 0.f, 1.f, 1.f);
	else if (status == 2)//lookat
		spriteRenderer.Color = glm::vec4(1.f, 0.4f, 0.1f, 1.f);
	else//default
		spriteRenderer.Color = glm::vec4(0.f, 1.f, 0.f, 1.f);
}

void Map::UpdateMap(Ref<Scene> scene, int x, int y, int status) {
	ChangeColor(scene, x, y, status);
}

void Map::DestroyFloor(Ref<Scene> scene, int i, int j)
{
	scene->DestroyEntity(_floor[i][j]);
}

glm::ivec2 Map::FindFloor(glm::vec3 lookat, glm::vec3 pos) {
	glm::ivec2 ans(-1,-1);

	bool in_range = 1;
	glm::vec3 FloorPos = pos;
	FloorPos += glm::vec3(48.f, 0.f, 48.f);
	while (in_range == 1) {
		FloorPos += lookat;
		if (FloorPos.z > 4896.f || FloorPos.z < 0 || FloorPos.x>4896 || FloorPos.x < 0 || FloorPos.y > 100000)
			in_range = 0;
		if (FloorPos.y < 0)break;
	}
	if (in_range == 0)return ans;
	//glm::vec3 FloorPos = glm::vec3(pos.x - (lookat.x * distance), 0.f, pos.z - (lookat.z * distance));
	ans.x = int(FloorPos.x) / 96;
	ans.y = int(FloorPos.z) / 96;
	TRACE("{0} {1}", ans.x, ans.y);
	return ans;
}