#include "World.h"

#include <iostream>
#include "glm/gtc/random.hpp"

int BlockStatus[51][51] = { 0 };

World::World()
{
	// Create scene
	m_Scene = CreateRef<Scene>("Demo");
	AssetLibrary::Initialize(m_Scene);

	// Default lookat block
	PS = glm::ivec2(-1, -1);
	NS = PS;
	Building = PS;

	// Default Warning Notification
	w = new Warning(m_Scene, 0);
	BuildWarning = new Warning(m_Scene, 0);

	// 簡易的示範程式碼，我把程式碼移到下面了，不然很亂
	m_Camera = CreateCamera();
	m_Text = CreateText(); // 創造2D文字的範例
	m_Music = CreateMusic();
	m_Sound = CreateSound();

	//create floor 51*51
	m_map = new Map(m_Scene, 51);

	NowBuilding = new ArcherTower(m_Scene);
	NowBuilding->setPosition(glm::vec3(0, 0, 0));
	NowBuilding->setlevel(0, m_Scene);
	NowBuilding->Destroy(m_Scene);

	bg = new BabyGrogu(m_Scene, 1);
	DefaultDir = bg->getRotation();
	bg->setPosition(glm::vec3(96.f * 3, -8000, 96.f * 0));

	for (int i = 0;i < 51;i++) {
		for (int j = 0;j < 51;j++) {
			towers[i][j] = NULL;
			m_map->pathinfo[i][j] = glm::ivec2(25, 25);
		}
	}
}

World::~World()
{
	AssetLibrary::ShutDown();
}

float speed = 700.0f;
void World::OnUpdate(float dt) // dt現在是正確的了!
{
	auto& cameraTransform = m_Camera.GetComponent<TransformComponent>();

	// Mouse control
	static glm::vec2 prevPos;
	glm::vec2 mousePos = IO::GetMousePosition(); // IO:滑鼠位置與是否按下
	glm::vec2 delta = mousePos - prevPos;
	prevPos = mousePos;

	static float yaw = 180.0f, pitch = 0.0f;
	yaw += 100.0f * delta.x * dt;
	//yaw = 1.5f * yaw;
	pitch -= 100.0f * delta.y * dt;
	//pitch = 1.5f * pitch;

	if (pitch > 89.99f) pitch = 89.99f;
	if (pitch < -89.99f) pitch = -89.99f;

	cameraTransform.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	cameraTransform.Rotation = glm::rotate(cameraTransform.Rotation, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	cameraTransform.Rotation = glm::rotate(cameraTransform.Rotation, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	

	w->update(m_Scene, dt);
	BuildWarning->update(m_Scene, dt);


	// Rotation to direction
	#if 1
	glm::vec3 dir = glm::inverse(glm::toMat4(cameraTransform.Rotation)) * glm::vec4(0.f, 0.f, 1.f, 0.0f);
	glm::vec3 lookat = dir;
	dir.y = 0;
	dir = glm::normalize(dir);
	lookat = glm::normalize(lookat);

	if (IO::IsKeyPressed(KeyCode::W))
		cameraTransform.Position += speed * dir * dt;
	if (IO::IsKeyPressed(KeyCode::S))
		cameraTransform.Position -= speed * dir * dt;
	if (IO::IsKeyPressed(KeyCode::A))
		cameraTransform.Position -= speed * glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)) * dt;
	if (IO::IsKeyPressed(KeyCode::D))
		cameraTransform.Position += speed * glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)) * dt;
	if (IO::IsKeyPressed(KeyCode::Space))
		if (cameraTransform.Position.y < 512) cameraTransform.Position.y += speed * dt;
	if (IO::IsKeyPressed(KeyCode::LeftShift))
		if (cameraTransform.Position.y > 10) cameraTransform.Position.y -= speed * dt;
	#endif

	// Detect which block is aiming
	#if 1
	NS = m_map->FindFloor(lookat, cameraTransform.Position);
	if (PS.x != -1 && PS != NS)
		m_map->ChangeColor(m_Scene, PS.x, PS.y, m_map->mapinfo[PS.x][PS.y]);
	PS = NS;
	if (NS.x != -1)
		m_map->ChangeColor(m_Scene, NS.x, NS.y, 2);
	#endif

	// FPS counter
	#if 1
	static float TotalTime = 0;
	static float time = 0;
	static int cnt = 0;
	TotalTime += dt;
	time += dt;
	cnt++;
	if (time > 1.0f)
	{
		auto& text = m_Text.GetComponent<TextRendererComponent>();
		text.Text = "FPS:" + std::to_string(cnt);
		time -= 1.0f;
		cnt = 0;
	}
	#endif

	// Some sound stuff
	#if 1
	static bool isMusicPlaying = false;
	if (IO::IsKeyDown(KeyCode::M))
	{
		if (isMusicPlaying)
			m_Music.GetComponent<MusicSourceComponent>().Stop = true;
		else
			m_Music.GetComponent<MusicSourceComponent>().Begin = true;
		isMusicPlaying = !isMusicPlaying;
	}
	if (IO::IsKeyReleased(KeyCode::R))
		m_Sound.GetComponent<SoundSourceComponent>().Begin = true;
	#endif

	// Some construct stuff
	#if 1
	if (IO::IsKeyDown(KeyCode::C))
	{
		if (Building != glm::ivec2(-1, -1)) {
			BuildWarning->Destroy(m_Scene);
			delete BuildWarning;
			BuildWarning = new Warning(m_Scene, 3);
		}
		else {
			Building = m_map->FindFloor(lookat, cameraTransform.Position);
			if (m_map->mapinfo[Building.x][Building.y] != 0) {
				BuildWarning->Destroy(m_Scene);
				delete BuildWarning;
				BuildWarning = new Warning(m_Scene, 2);
				Building = glm::ivec2(-1, -1);
			}
			else {
				//start building
				delete NowBuilding;
				NowBuilding = new ArcherTower(m_Scene);
				NowBuilding->setPosition(glm::vec3(Building.x * 96, 0, Building.y * 96));
				NowBuilding->setlevel(1, m_Scene);
				NowBuilding->setTransparent();
			}
		}
	}
	if (IO::GetMouseScroll() == -1 && Building != glm::ivec2(-1, -1)) {
		BuildingIdx += 4;
		BuildingIdx %= 5;
		NowBuilding->Destroy(m_Scene);
		switch (BuildingIdx) {
		case 0: {
			delete NowBuilding;
			NowBuilding = new ArcherTower(m_Scene);
			break;
		}
		case 1: {
			delete NowBuilding;
			NowBuilding = new BallistaTower(m_Scene);
			break;
		}
		case 2: {
			delete NowBuilding;
			NowBuilding = new CannonTower(m_Scene);
			break;
		}
		case 3: {
			delete NowBuilding;
			NowBuilding = new PoisonTower(m_Scene);
			break;
		}
		case 4: {
			delete NowBuilding;
			NowBuilding = new WizardTower(m_Scene);
			break;
		}
		}
		NowBuilding->setPosition(glm::vec3(Building.x * 96, 0, Building.y * 96));
		NowBuilding->setlevel(1, m_Scene);
		NowBuilding->setTransparent();
	}
	if (IO::GetMouseScroll() == 1 && Building != glm::ivec2(-1, -1)) {
		BuildingIdx += 1;
		BuildingIdx %= 5;
		NowBuilding->Destroy(m_Scene);
		switch (BuildingIdx) {
		case 0: {
			delete NowBuilding;
			NowBuilding = new ArcherTower(m_Scene);
			break;
		}
		case 1: {
			delete NowBuilding;
			NowBuilding = new BallistaTower(m_Scene);
			break;
		}
		case 2: {
			delete NowBuilding;
			NowBuilding = new CannonTower(m_Scene);
			break;
		}
		case 3: {
			delete NowBuilding;
			NowBuilding = new PoisonTower(m_Scene);
			break;
		}
		case 4: {
			delete NowBuilding;
			NowBuilding = new WizardTower(m_Scene);
			break;
		}
		}
		NowBuilding->setPosition(glm::vec3(Building.x * 96, 0, Building.y * 96));
		NowBuilding->setlevel(1, m_Scene);
		NowBuilding->setTransparent();
	}
	if (IO::IsMouseButtonDown(MouseButton::Right) && Building != glm::ivec2(-1, -1)) {
		NowBuilding->Destroy(m_Scene);
		switch (BuildingIdx) {
		case 0: {
			towers[Building.x][Building.y] = new ArcherTower(m_Scene);
			break;
		}
		case 1: {
			towers[Building.x][Building.y] = new BallistaTower(m_Scene);
			break;
		}
		case 2: {
			towers[Building.x][Building.y] = new CannonTower(m_Scene);
			break;
		}
		case 3: {
			towers[Building.x][Building.y] = new PoisonTower(m_Scene);
			break;
		}
		case 4: {
			towers[Building.x][Building.y] = new WizardTower(m_Scene);
			break;
		}
		}
		towers[Building.x][Building.y]->setPosition(glm::vec3(Building.x * 96, 0, Building.y * 96));
		towers[Building.x][Building.y]->setlevel(1, m_Scene);
		m_map->ChangeColor(m_Scene, Building.x, Building.y, 1);
		Building = glm::ivec2(-1, -1);
	}
	#endif

	// Mob path and building destroy
	if (TotalTime > PathPeriod)
	{
		int lvl = 1;
		glm::ivec2 spawn = m_map->DecidePath(m_Scene);
		m_SpawnList.push_back(spawn);
		//TRACE("{0} {1}", spawn.x, spawn.y);
		TotalTime -= PathPeriod;
		ClearMap();
		for(auto it = m_SpawnList.begin();it!=m_SpawnList.end();it++)
			MobSpawn(*it, lvl);
	}
	MobMove();

	m_Scene->OnUpdate(dt); // Renders the scene
}


void World::MobSpawn(glm::ivec2 spawn, int lvl) {
	int MobIdx = glm::linearRand(1, 6);
	Enemy* mob = NULL;
	switch (MobIdx) {
		case 1: {//BlueDragon
			mob = new BlueDragon(m_Scene, 1);
			break;
		}
		case 3: {//MonsterSkull
			mob = new MonsterSkull(m_Scene, 1);
			break;
		}
		case 4: {//ShadowDragon
			mob = new ShadowDragon(m_Scene, 1);
			break;
		}
		case 5: {//SnowDragon
			mob = new SnowDragon(m_Scene, 1);
			break;
		}
		case 6: {//WhiteChineseDragon
			mob = new WhiteChineseDragon(m_Scene, 1);
			break;
		}
		case 7: {//Yoda
			mob = new BabyGrogu(m_Scene, 1);
			//init face toward +z
			break;
		}
		case 8: {//Patrick
			mob = new Patrick(m_Scene, 1);
			break;
		}
	}
	mob->setPosition(glm::vec3(96.f * spawn.x, 80, 96.f * spawn.y));
	m_MobMap[mob] = spawn;
}

void World::MobMove()
{
	for (auto it = m_MobMap.begin(); it != m_MobMap.end();it++) {
		Enemy* mob = it->first;
		int MobIdx = mob->_type;
		glm::ivec2 map = it->second;
		glm::vec3 MobPos = mob->getPosition();

		glm::vec3 goal = glm::vec3(m_map->pathinfo[map.x][map.y].x * 96, MobPos.y, m_map->pathinfo[map.x][map.y].y * 96);
		glm::vec3 delta = goal - MobPos;
		glm::vec3 dir = glm::normalize(delta);
		float LR, angle;
		glm::quat rot;
		switch (MobIdx) {
			default: {
				LR = (dir.x > 0) ? 1.f : -1.f;
				angle = glm::dot(dir, glm::vec3(0.f, 0.f, 1.f));
				rot = glm::rotate(DefaultDir, LR * glm::acos(angle), glm::vec3(0.0f, 0.0f, 1.0f));
				break;
			}
		}
		
		mob->setRotation(rot);
		if (glm::length(delta) > 40) {
			MobPos += dir * 0.25f;
			mob->setPosition(MobPos);
			m_MobMap[mob] = m_map->Pos2Map(MobPos);
		}
	}
}

void World::ClearMap()
{
	//super inefficient
	for (int i = 1;i < 50;i++)
		for (int j = 1;j < 50;j++) {
			if (m_map->mapinfo[i][j] == 4 && towers[i][j] != NULL) {
				towers[i][j]->Destroy(m_Scene);
				delete towers[i][j];
				towers[i][j] = NULL;
			}
		}
}

Entity World::CreateCamera()
{
	// Create a camera
	Entity camera = m_Scene->CreateEntity("Camera");
	PerspectiveCameraSpecification specs;
	specs.FarPlane = 2500.f;
	camera.AddComponent<CameraComponent>(Camera(specs));

	auto& transform = camera.GetComponent<TransformComponent>();
	transform.Position = { 0.0f, 0.0f, 100.f };
	transform.Rotation = glm::quat(glm::vec3{ 0.0f, 180.0f, 0.0f });

	return camera;
}

Entity World::CreateSprite()
{
	// Create a blue quad
	Entity quad = m_Scene->CreateEntity("Blue quad");

	quad.AddComponent<SpriteRendererComponent>(glm::vec2{ 200.f, 200.f }, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f }); // AddComponent 賦予性質

	auto& transform = quad.GetComponent<TransformComponent>();	//GetComponent 取得性質
	transform.Position = { -300, 300, 0 };

	return quad;
}

Entity World::CreateSpriteWithTexture(const std::string& path)
{
	// Create quad with texture
	Entity quad = m_Scene->CreateEntity("Red quad");

	AssetHandle tex = AssetManager::LoadTexture(path); // 取得Texture轉成數字(AssetHandle)

	auto& spriteRenderer = quad.AddComponent<SpriteRendererComponent>(); // AddComponent 賦予性質
	spriteRenderer.Size = glm::vec2(200.f, 200.f);
	spriteRenderer.Color = glm::vec4(1.0f);
	spriteRenderer.Texture = tex;

	auto& UI = quad.AddComponent<UIElementComponent>(); // 加上UIElementComponent把3D物件變成2D，體位置由下面三行決定，Transform.Position沒用了
	UI.Anchor = { 0, 1 };
	UI.Pivot = { 0, 1 };
	UI.Offset = { 0, -100 };

	auto& button = quad.AddComponent<ButtonComponent>();
	button.ButtonDownCallback = []() { TRACE("Button is pressed"); };

	auto& transform = quad.GetComponent<TransformComponent>();
	transform.Position = { -300, -300, 0 }; // 不會影響物體位置(被UIElementComponent覆蓋)

	return quad;
}

Entity World::CreateFloor(int i, int j) {
	Entity quad = m_Scene->CreateEntity("Green Floor");

	//AssetHandle tex = AssetManager::LoadTexture(path); // 取得Texture轉成數字(AssetHandle)

	auto& spriteRenderer = quad.AddComponent<SpriteRendererComponent>(); // AddComponent 賦予性質
	spriteRenderer.Size = glm::vec2(94.f, 94.f);
	if (BlockStatus[i][j])
		spriteRenderer.Color = glm::vec4(0.f, 0.f, 1.f, 1.f);
	else
		spriteRenderer.Color = glm::vec4(0.f, 1.f, 0.f, 1.f);
	//spriteRenderer.Texture = tex;

	auto& transform = quad.GetComponent<TransformComponent>();
	transform.Position = { i * 96, 0, j * 96 }; // 不會影響物體位置(被UIElementComponent覆蓋)
	transform.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	transform.Rotation = glm::rotate(transform.Rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	return quad;
}

Entity World::LoadModel()
{
	Entity model = Model::Load(m_Scene, "Model/colorball/colorball.dae"); // 載入模型
	auto& transform = model.GetComponent<TransformComponent>();
	transform.Position.x = 00.f;
	transform.Scale *= 10.1f;

	return model;
}

Entity World::LoadModelWithTexture()
{
	AssetHandle albedo = AssetManager::LoadTexture("Model/stormtrooper/textures/Stormtrooper_D.png"); // albedo是最基礎的顏色
	AssetHandle mat1 = AssetManager::CreateMaterial(albedo); // 用texture產生material

	Entity model = Model::LoadSkinned(m_Scene, "Model/stormtrooper/source/silly_dancing.fbx", { mat1 }); // 額外輸入material，有些模型需邀超過一個(Console會說)，要注意

	auto& transform = model.GetComponent<TransformComponent>();
	transform.Position.x = 100.f;
	transform.Scale = glm::vec3(50.0f);

	auto& animator = model.GetComponent<AnimatorComponent>();
	animator.IsPlaying = true;
	animator.AnimationIndex = 0;

	return model;
}

Entity World::CreateModelByCopying(Entity copySource)
{
	Entity model = m_Scene->CreateEntity(copySource, "Name for newly created model");

	auto& transform = model.GetComponent<TransformComponent>();
	transform.Position.x += 100.f;

	return model;
}

Entity World::CreateText()
{
	Entity text = m_Scene->CreateEntity("Text");

	auto& transform = text.GetComponent<TransformComponent>();
	transform.Scale *= 2.f;


	auto& UI = text.AddComponent<UIElementComponent>(); //加UIElementComponent讓物體變成2D的，物體位置由下面三行決定，Transform.Position沒用了
	UI.Anchor = glm::vec2(-1.0f, -1.0f);
	UI.Pivot = glm::vec2(-1.0f, -1.0f);
	UI.Offset = glm::vec2(10.0f, 10.0f);

	auto& textRenderer = text.AddComponent<TextRendererComponent>();
	textRenderer.Text = "FPS:???";
	textRenderer.Font = AssetLibrary::Get(Asset::OpenSansFont);
	textRenderer.FontSize = 20.0f;
	textRenderer.Size = glm::vec2(200.0f, 100.0f);
	textRenderer.OutlineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	textRenderer.OutlineThickness = 0.16f; // In the range [0, 0.3f] (not sure)

	return text;
}

Entity World::CreateMusic()
{
	Entity music = m_Scene->CreateEntity("Music");
	auto& source = music.AddComponent<MusicSourceComponent>();
	source.Music = AssetManager::LoadMusic("Sound/music.mp3");
	source.Volume = 0.9f;
	source.FadeIn = 2.0f;
	source.FadeOut = 5.0f;
	source.Begin = true;

	return music;
}

Entity World::CreateSound()
{
	Entity sound = m_Scene->CreateEntity("sound");
	auto& source = sound.AddComponent<SoundSourceComponent>();
	source.Sound = AssetManager::LoadSound("Sound/scary-swoosh-142323.mp3");
	source.Volume = 1.0f;
	source.FadeIn = 0.0f;
	source.FadeOut = 0.0f;

	return sound;
}

