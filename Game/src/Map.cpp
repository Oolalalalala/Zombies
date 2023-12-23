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
	if (mapinfo[i][j])
		spriteRenderer.Color = glm::vec4(0.f, 0.f, 1.f, 1.f);
	else
		spriteRenderer.Color = glm::vec4(0.f, 1.f, 0.f, 1.f);

	auto& transform = quad.GetComponent<TransformComponent>();
	transform.Position = { i * 96, 0, j * 96 }; // 不會影響物體位置(被UIElementComponent覆蓋)
	transform.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	transform.Rotation = glm::rotate(transform.Rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	return quad;
}

void Map::UpdateMap(Ref<Scene> scene, int x, int y, int status) {
	mapinfo[x][y] = status;
	DestroyFloor(scene, x, y);
	_floor[x][y] = CreateFloor(scene, x, y);
}
void Map::DestroyFloor(Ref<Scene> scene, int i, int j)
{
	scene->DestroyEntity(_floor[i][j]);
}
