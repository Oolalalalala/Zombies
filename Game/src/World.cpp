#include "World.h"

#include <iostream>
#include "glm/gtc/random.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "AssetLibrary.h"

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

	//create floor 51*51
	m_map = new Map(m_Scene, 51);

	// 簡易的示範程式碼，我把程式碼移到下面了，不然很亂
	m_Camera = CreateCamera();
	m_Text = CreateText(); // 創造2D文字的範例
	m_Music = CreateMusic();
	m_Sound = CreateSound();
	m_SoundEffect1 = CreateSoundEffect(1);
	m_SoundEffect2 = CreateSoundEffect(2);
	m_SoundEffect3 = CreateSoundEffect(3);
	m_SoundEffect4 = CreateSoundEffect(4);

	NowBuilding = new ArcherTower(m_Scene);
	NowBuilding->setPosition(glm::vec3(0, 0, 0));
	NowBuilding->setlevel(0, m_Scene);
	NowBuilding->Destroy(m_Scene);


	for (int i = 0;i < 51;i++) {
		for (int j = 0;j < 51;j++) {
			towers[i][j] = NULL;
			m_map->pathinfo[i][j] = glm::ivec2(25, 25);
		}
	}
	for (int i = 0;i < 9;i++) {
		df[i] = glm::quat(0.23, 0.23, 0.23, 0.23);
	}

	hpBar = new H_PBar(m_Scene);
	crystal = AssetLibrary::GetModel(Asset::stylized_crystal);
	crystal.GetComponent<MeshRendererComponent>().Enabled = true;
	auto& transform = crystal.GetComponent<TransformComponent>();
	transform.Position = { 2400, 00, 2400 };
	transform.Scale *= 3.0f;

	m_Time = CreateTime();
	gold = new Gold(m_Scene);

	IO::SetCursorVisibility(false);
	m_IsCursorMode = false;
}

World::~World()
{
	AudioMixer::StopMusic(AssetManager::GetMusic(m_Music.GetComponent<MusicSourceComponent>().Music));

	delete m_map;

	for (int i = 0;i < 51;i++) {
		for (int j = 0;j < 51;j++) {
			if (towers[i][j] != NULL)delete towers[i][j];
		}
	}
	delete NowBuilding;
	delete w;
	delete BuildWarning;

	delete hpBar;
	delete gold;
	AssetLibrary::ShutDown();

	IO::SetCursorVisibility(true);
}

float speed = 700.0f;
void World::OnUpdate(float dt) // dt現在是正確的了!
{
	auto& cameraTransform = m_Camera.GetComponent<TransformComponent>();

	if (IO::IsKeyDown(KeyCode::Escape))
	{
		m_IsCursorMode = !m_IsCursorMode;
		IO::SetCursorVisibility(m_IsCursorMode);
	}
	
	glm::vec3 dir = glm::inverse(glm::toMat4(cameraTransform.Rotation)) * glm::vec4(0.f, 0.f, 1.f, 0.0f);
	glm::vec3 lookat = dir;
	lookat = glm::normalize(lookat);
	dir.y = 0;
	dir = glm::normalize(dir);

	if (!m_IsCursorMode)
	{
		// Mouse control
		static glm::vec2 prevPos = IO::GetMousePosition();
		glm::vec2 newPos = IO::GetMousePosition();
		glm::vec2 delta = newPos - prevPos;
		prevPos = newPos;


		// Wrap the cursor around
		glm::vec2 windowSize = IO::GetWindowSize();
		if (prevPos.x <= 0)
		{
			IO::SetCursorPosition({ windowSize.x - 2, prevPos.y });
			prevPos = { windowSize.x - 2, prevPos.y };
		}
		if (prevPos.x >= windowSize.x - 1)
		{
			IO::SetCursorPosition({ 1, prevPos.y });
			prevPos = { 1, prevPos.y };
		}
		if (prevPos.y <= 0)
		{
			IO::SetCursorPosition({ prevPos.x, windowSize.y - 2 });
			prevPos = { prevPos.x, windowSize.y - 2 };
		}
		if (prevPos.y >= windowSize.y - 1)
		{
			IO::SetCursorPosition({ prevPos.x, 1 });
			prevPos = { prevPos.x, 1 };
		}

		static float yaw = 180.0f, pitch = 0.0f;
		yaw += 100.0f * delta.x * dt;
		pitch -= 100.0f * delta.y * dt;

		if (pitch > 89.99f) pitch = 89.99f;
		if (pitch < -89.99f) pitch = -89.99f;

		cameraTransform.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		cameraTransform.Rotation = glm::rotate(cameraTransform.Rotation, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		cameraTransform.Rotation = glm::rotate(cameraTransform.Rotation, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));



		// Rotation to direction
#if 1

	if (IO::IsKeyPressed(KeyCode::W))
		cameraTransform.Position += speed * dir * dt;
	if (IO::IsKeyPressed(KeyCode::S))
		cameraTransform.Position -= speed * dir * dt;
	if (IO::IsKeyPressed(KeyCode::A))
		cameraTransform.Position -= speed * glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)) * dt;
	if (IO::IsKeyPressed(KeyCode::D))
		cameraTransform.Position += speed * glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)) * dt;
	if (IO::IsKeyPressed(KeyCode::Space))
		if (cameraTransform.Position.y < 2048) cameraTransform.Position.y += speed * dt;
	if (IO::IsKeyPressed(KeyCode::LeftShift))
		if (cameraTransform.Position.y > -110) cameraTransform.Position.y -= speed * dt;
	#endif
	}

	
	w->update(m_Scene, dt);
	BuildWarning->update(m_Scene, dt);



	// temp
	static bool pause = false;
	if (IO::IsKeyDown(KeyCode::P))
		pause = !pause;
	if (pause)
		return;

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
	static float PathTime = 0;
	static float time = 0;
	static int cnt = 0;
	TotalTime += dt;
	PathTime += dt;
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
			m_SoundEffect3.GetComponent<SoundSourceComponent>().Begin = true;
		}
		else {
			Building = m_map->FindFloor(lookat, cameraTransform.Position);
			if (m_map->mapinfo[Building.x][Building.y] != 0) {
				BuildWarning->Destroy(m_Scene);
				delete BuildWarning;
				BuildWarning = new Warning(m_Scene, 2);
				m_SoundEffect4.GetComponent<SoundSourceComponent>().Begin = true;
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
		if (gold->GetGold() < 100)
		{
			w->Destroy(m_Scene);
			delete w;
			w = new Warning(m_Scene, 1);
			m_SoundEffect3.GetComponent<SoundSourceComponent>().Begin = true;
		}
		else
		{
			gold->UpdateGold(-100);
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
			for (auto it = m_MobMap.begin();it != m_MobMap.end();it++) {
				Enemy* mob = it->first;
				glm::vec3 MobPos = mob->getPosition();
				glm::vec3 TowerPos = glm::vec3(Building.x * 96, MobPos.y, Building.y * 96);
				float d2 = glm::length(MobPos - TowerPos);
				if (d2 - 768 < 0) {//in
					towers[Building.x][Building.y]->AddTrackingEnemy(mob);
				}
			}
			towers[Building.x][Building.y]->setPosition(glm::vec3(Building.x * 96, 0, Building.y * 96));
			towers[Building.x][Building.y]->setlevel(1, m_Scene);
			m_map->ChangeColor(m_Scene, Building.x, Building.y, 1);
			Building = glm::ivec2(-1, -1);
		}
	}
	#endif

	//Upgrade Tower
	if (IO::IsKeyDown(KeyCode::U) && NS.x != -1 && towers[NS.x][NS.y] != NULL) {
		int lvl = towers[NS.x][NS.y]->getLevel();
		int cost = int(100 * glm::pow(0.9, lvl));
		if (lvl != 4 && gold->GetGold(); gold->GetGold() >= cost) {
			towers[NS.x][NS.y]->setlevel(lvl + 1, m_Scene);
			gold->UpdateGold(-cost);
		}
		else if (lvl == 4) {
			w->Destroy(m_Scene);
			delete w;
			w = new Warning(m_Scene, 4);
			m_SoundEffect4.GetComponent<SoundSourceComponent>().Begin = true;
		}
		else {
			w->Destroy(m_Scene);
			delete w;
			w = new Warning(m_Scene, 1);
			m_SoundEffect4.GetComponent<SoundSourceComponent>().Begin = true;
		}
	}

	// Mob path and building destroy
	static int lvl = 1;
	lvl = 1 + int(TotalTime / LvlUpPeriod);
	//static int temp = 0;
	if (PathTime > PathPeriod)
	{
		//temp++;
		//if (temp == 6)temp++;
		glm::ivec2 spawn = m_map->DecidePath(m_Scene);
		if(m_SpawnList.size()<=20)m_SpawnList.push_back(spawn);
		PathTime -= PathPeriod;
		ClearMap();
		for(auto it = m_SpawnList.begin();it!=m_SpawnList.end();it++) MobSpawn(*it, lvl);
		//if (temp <= 8)MobSpawn(spawn, temp);
		//if (temp == 8)temp = 0;
	}
	MobMove(dt);
	for (int i = 0;i < 51;i++) {
		for (int j = 0;j < 51;j++) {
			if(towers[i][j]!=NULL)towers[i][j]->OnUpdate(dt);
		}
	}


	hpBar->UpdateH_PBar(m_Camera.GetComponent<TransformComponent>().Position);
	hpBar->TakeDamage(-10 * dt);

	static float money = 0;
	money += dt * 10;
	if (money > 1)
	{
		money--;
		gold->UpdateGold(1);
	}

	auto& text = m_Time.GetComponent<TextRendererComponent>();
	text.Text = "TIME:" + std::to_string(int(TotalTime)) + " / Lvl:" + std::to_string(lvl);


	w->update(m_Scene, dt);
	BuildWarning->update(m_Scene, dt);

	m_Scene->OnUpdate(dt); // Renders the scene
}


void World::MobSpawn(glm::ivec2 spawn, int lvl) {
	int MobIdx = glm::linearRand(1, 8);
	//lvl = 1;
	Enemy* mob = NULL;
	switch (MobIdx) {
		case 1: {//BlueDragon
			mob = new BlueDragon(m_Scene, lvl);
			break;
		}
		case 2: {
			mob = new Monster(m_Scene, lvl);
			break;
		}
		case 3: {//MonsterSkull
			mob = new MonsterSkull(m_Scene, lvl);
			break;
		}
		case 4: {//ShadowDragon
			mob = new ShadowDragon(m_Scene, lvl);
			break;
		}
		case 5: {//SnowDragon
			mob = new SnowDragon(m_Scene, lvl);
			break;
		}
		case 6: {//WhiteChineseDragon
			mob = new WhiteChineseDragon(m_Scene, lvl);
			break;
		}
		case 7: {//Yoda
			mob = new BabyGrogu(m_Scene, 1);
			break;
		}
		default: {//Patrick
			mob = new Patrick(m_Scene, lvl);
			break;
		}
	}
	mob->_type = MobIdx;
	mob->setPosition(glm::vec3(96.f * spawn.x, 80, 96.f * spawn.y));
	m_MobMap[mob] = spawn;
	if (df[MobIdx] == glm::quat(0.23, 0.23, 0.23, 0.23))df[MobIdx] = mob->getRotation();
}

void World::MobMove(float dt)
{
	for (auto it = m_MobMap.begin(); it != m_MobMap.end();) {
		Enemy* mob = it->first;
		if (mob->gethp() <= 0) {
			for (int i = 0;i < 51;i++) {
				for (int j = 0;j < 51;j++) {
					if (towers[i][j] != NULL) {
						towers[i][j]->RemoveTrackingEnemy(mob);
					}
				}
			}
			gold->UpdateGold(mob->getLevel()*50);
			mob->Destroy(m_Scene);
			delete mob;
			it = m_MobMap.erase(it);
		}
		else {
			int MobIdx = mob->_type;
			glm::ivec2 map = it->second;
			glm::vec3 MobPos = mob->getPosition();

			glm::vec3 goal = glm::vec3(m_map->pathinfo[map.x][map.y].x * 96, MobPos.y, m_map->pathinfo[map.x][map.y].y * 96);
			glm::vec3 goal2 = glm::vec3(2400, MobPos.y, 2400);

			glm::vec3 delta = goal - MobPos;
			glm::vec3 delta2 = goal2 - MobPos;
			glm::vec3 dir = glm::normalize(delta);
			float LR, cos;
			glm::quat rot;
			switch (MobIdx) {
			case 8: {//Patrick
				LR = (dir.x < 0) ? -1.f : 1.f;
				cos = glm::dot(dir, glm::vec3(0.f, 0.f, 1.f));
				rot = glm::rotate(df[8], LR * glm::acos(cos), glm::vec3(0.f, 1.f, 0.f));
				break;
			}
			case 6: {//WhiteChineseDragon
				LR = (dir.z < 0) ? 1.f : -1.f;
				cos = glm::dot(dir, glm::vec3(1.f, 0.f, 0.f));
				rot = glm::rotate(df[6], LR * glm::acos(cos), glm::vec3(0.f, 1.f, 0.f));
				break;
			}
			case 5: {//SnowDragon
				LR = (dir.x < 0) ? -1.f : 1.f;
				cos = glm::dot(dir, glm::vec3(0.f, 0.f, 1.f));
				rot = glm::rotate(df[5], LR * glm::acos(cos), glm::vec3(0.f, 1.f, 0.f));
				break;
			}
			case 1: {//BlueDragon
				rot = glm::rotate(df[1], glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
				LR = (dir.x > 0) ? -1.f : 1.f;
				cos = glm::dot(dir, glm::vec3(0.f, 0.f, -1.f));
				rot = glm::rotate(rot, LR * glm::acos(cos), glm::vec3(-1.f, 0.f, 0.f));
				break;
			}
			case 3: {//MonsterSkull
				rot = glm::rotate(df[3], glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
				LR = (dir.x < 0) ? -1.f : 1.f;
				cos = glm::dot(dir, glm::vec3(0.f, 0.f, 1.f));
				rot = glm::rotate(rot, LR * glm::acos(cos), glm::vec3(-1.f, 0.f, 0.f));
				break;
			}
			case 4: {//ShadowDragon
				rot = glm::rotate(df[4], glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
				LR = (dir.z < 0) ? 1.f : -1.f;
				cos = glm::dot(dir, glm::vec3(1.f, 0.f, 0.f));
				rot = glm::rotate(rot, LR * glm::acos(cos), glm::vec3(0.f, -1.f, 0.f));
				break;
			}
			case 2: {//Monster
				rot = glm::rotate(df[2], glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
				LR = (dir.z < 0) ? 1.f : -1.f;
				cos = glm::dot(dir, glm::vec3(1.f, 0.f, 0.f));
				rot = glm::rotate(rot, LR * glm::acos(cos), glm::vec3(0.f, 0.f, 1.f));
				break;
			}
			case 7: {//Yoda
				rot = glm::rotate(df[7], glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
				LR = (dir.x > 0) ? 1.f : -1.f;
				cos = glm::dot(dir, glm::vec3(0.f, 0.f, 1.f));
				rot = glm::rotate(rot, LR * glm::acos(cos), glm::vec3(0.f, 0.f, 1.f));
				break;
			}
			}

			mob->setRotation(rot);
			
			glm::ivec2 Mobmap = m_map->Pos2Map(MobPos);
			glm::vec3 OldMobPos = MobPos;
			if (glm::length(delta) > 40 && glm::length(delta2) >240) {
				MobPos += dir * dt * 400.f;
			}
			if (glm::length(delta2) <= 240) {
				hpBar->TakeDamage(mob->getDamage()*dt);
				if (hpBar->GetHp() <= 5000.f && go == false) {
					m_SoundEffect1.GetComponent<SoundSourceComponent>().Begin = true;
					go = 1;
				}
				if (hpBar->GetHp() <= 2500.f && nervous == false) {
					m_SoundEffect2.GetComponent<SoundSourceComponent>().Begin = true;
					nervous = 1;
				}
				if (hpBar->GetHp() <= 0.0) {
					m_EndGameCallback(TotalTime);
				}
			}
			mob->setPosition(MobPos);
			
			m_MobMap[mob] = m_map->Pos2Map(MobPos);
			for (int i = 0;i < 51;i++) {
				for (int j = 0;j < 51;j++) {
					if (towers[i][j] != NULL) {
						glm::vec3 TowerPos(i * 96, OldMobPos.y, j * 96);
						float d1 = glm::length(OldMobPos - TowerPos);
						float d2 = glm::length(MobPos - TowerPos);
						if (d1 - 768 > 0 && d2 - 768 < 0) {//in
							towers[i][j]->AddTrackingEnemy(mob);
						}
						else if (d1 - 768 < 0 && d2 - 768 > 0) {//out
							towers[i][j]->RemoveTrackingEnemy(mob);
						}
					}
				}
			}
			it++;
		}
		
	}
}

void World::SetEndGameCallback(std::function<void(int)> callback)
{
	m_EndGameCallback = callback;
}

void World::ClearMap()
{
	//super inefficient
	for (int i = 1;i <= 50;i++)
		for (int j = 1;j <= 50;j++) {
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
	transform.Position = { 400.0f, 50.0f, 400.f };
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
	source.Volume = 0.5f;
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

Entity World::CreateSoundEffect(int idx)
{
	Entity soundef = m_Scene->CreateEntity("sound");
	auto& source = soundef.AddComponent<SoundSourceComponent>();
	switch (idx) {
	case 1: {
		source.Sound = AssetManager::LoadSound("Sound/go.mp3");
		break;
	}
	case 2: {
		source.Sound = AssetManager::LoadSound("Sound/nervous.mp3");
		break;
	}
	case 3: {
		source.Sound = AssetManager::LoadSound("Sound/think.mp3");
		break;
	}
	case 4: {
		source.Sound = AssetManager::LoadSound("Sound/whchj.mp3");
		break;
	}
	}
	source.Volume = 1.0f;
	source.FadeIn = 0.0f;
	source.FadeOut = 0.0f;

	return soundef;
}

Entity World::CreateTime()
{
	Entity Time= m_Scene->CreateEntity("Time");
	auto& transform = Time.GetComponent<TransformComponent>();
	transform.Scale *= 2.f;

	auto& UI = Time.AddComponent<UIElementComponent>(); //加UIElementComponent讓物體變成2D的，物體位置由下面三行決定，Transform.Position沒用了
	UI.Anchor = glm::vec2(-1.0f, 1.0f);
	UI.Pivot = glm::vec2(-1.0f, 1.0f);
	UI.Offset = glm::vec2(10.0f, -10.0f);

	auto& textRenderer = Time.AddComponent<TextRendererComponent>();
	textRenderer.Text = "TIME=0\nLvl=1";
	textRenderer.Font = AssetLibrary::Get(Asset::OpenSansFont);
	textRenderer.FontSize = 20.0f;
	textRenderer.Size = glm::vec2(100.0f, 30.0f);
	textRenderer.OutlineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	textRenderer.OutlineThickness = 0.16f; // In the range [0, 0.3f] (not sure)
	return Time;
}
 

