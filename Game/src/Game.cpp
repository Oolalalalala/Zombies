#include "Game.h"

#include <iostream>

int BlockStatus[51][51] = { 0 };

Game::Game()
	: Application()
{
	
}

Game::~Game()
{
}

void Game::OnCreate()
{

	// Create scene
	m_Scene = CreateRef<Scene>("Demo");
	AssetLibrary::Initialize(m_Scene);

	// Default lookat block
	PS = glm::ivec2(-1, -1);
	NS = PS;

	// 簡易的示範程式碼，我把程式碼移到下面了，不然很亂
	m_Camera = CreateCamera();
	m_Text = CreateText(); // 創造2D文字的範例
	m_Music = CreateMusic();
	m_Sound = CreateSound();

	
	bg=new BabyGrogu(m_Scene);
	bg->setPosition(glm::vec3(-1 * 96, 0, -10 * 96));

	//create floor 51*51
	m_map = new Map(m_Scene, 51);

	for (int i = 0; i < 4; i++) {
		towers[0][i] = new ArcherTower(m_Scene);
		towers[0][i]->setPosition(glm::vec3(1 * 96, 0, i * 96 + 96));
		towers[0][i]->setlevel(i, m_Scene);
		towers[0][i]->AddToTargetList(bg);
	}
	for (int i = 0; i < 4; i++) {
		towers[1][i] = new BallistaTower(m_Scene);
		towers[1][i]->setPosition(glm::vec3(2 * 96, 0, i * 96 + 96));
		towers[1][i]->setlevel(i, m_Scene);
		towers[1][i]->AddToTargetList(bg);
	}
	for (int i = 0; i < 4; i++) {
		towers[2][i] = new CannonTower(m_Scene);
		towers[2][i]->setPosition(glm::vec3(3 * 96, 0, i * 96 + 96));
		towers[2][i]->setlevel(i, m_Scene);
		towers[2][i]->AddToTargetList(bg);
	}
	for (int i = 0; i < 4; i++) {
		towers[3][i] = new PoisonTower(m_Scene);
		towers[3][i]->setPosition(glm::vec3(4 * 96, 0, i * 96 + 96));
		towers[3][i]->setlevel(i, m_Scene);
	}
	for (int i = 0; i < 4; i++) {
		towers[4][i] = new WizardTower(m_Scene);
		towers[4][i]->setPosition(glm::vec3(5 * 96, 0, i * 96 + 96));
		towers[4][i]->setlevel(i, m_Scene);
	}
	UpgradeTest = new WizardTower(m_Scene);
	UpgradeTest->setPosition(glm::vec3(-1 * 96, 0, -1 * 96));
	UpgradeTest->setlevel(0, m_Scene);

	w = new Warning(m_Scene, 0);


	//update MapStatus
	for (int i = 1;i < 6;i++) {
		for (int j = 1;j < 5;j++) {
			m_map->ChangeColor(m_Scene, i, j, 1);//tower
		}
	}

}

float speed = 70.0f;
void Game::OnUpdate(float dt) // dt現在是正確的了!
{
	auto& cameraTransform = m_Camera.GetComponent<TransformComponent>();

	// Mouse control
	static glm::vec2 prevPos;
	glm::vec2 mousePos = IO::GetMousePosition(); // IO:滑鼠位置與是否按下
	glm::vec2 delta = mousePos - prevPos;
	prevPos = mousePos;

	static float yaw = 180.0f, pitch = 0.0f;

	if (IO::IsMouseButtonPressed(MouseButton::Right))
	{
		yaw += 100.0f * delta.x * dt;
		pitch -= 100.0f * delta.y * dt;

		if (pitch > 89.99f) pitch = 89.99f;
		if (pitch < -89.99f) pitch = -89.99f;

		cameraTransform.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		cameraTransform.Rotation = glm::rotate(cameraTransform.Rotation, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		cameraTransform.Rotation = glm::rotate(cameraTransform.Rotation, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	w->update(m_Scene, dt);
	if (IO::IsKeyDown(KeyCode::U))
	{
		int lvl = UpgradeTest->getLevel();
		if (lvl == 4) {
			w->Destroy(m_Scene);
			delete w;
			w = new Warning(m_Scene, 1);
			lvl = 3;
		}
		UpgradeTest->setlevel(lvl + 1, m_Scene);
	}
	if (IO::IsKeyDown(KeyCode::F))speed = (speed == 700.f) ? 700.0f : speed + 70.0f;
	if (IO::IsKeyDown(KeyCode::V))speed = 70.0f;
	if (IO::IsKeyDown(KeyCode::L))m_map->DecidePath(m_Scene);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			towers[i][j]->OnUpdate(dt);
		}
	}
	
	// Rotation to direction
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
		cameraTransform.Position.y += speed * dt;
	if (IO::IsKeyPressed(KeyCode::LeftShift))
		cameraTransform.Position.y -= speed * dt;

	// Detect which block is aiming
	NS = m_map->FindFloor(lookat, cameraTransform.Position);
	if (PS.x != -1 && PS != NS)
		m_map->ChangeColor(m_Scene, PS.x, PS.y, m_map->mapinfo[PS.x][PS.y]);
	PS = NS;
	if (NS.x != -1)
		m_map->ChangeColor(m_Scene, NS.x, NS.y, 2);
	
	// FPS counter
	static float time = 0;
	static int cnt = 0;
	time += dt;
	cnt++;
	if (time > 1.0f)
	{
		auto& text = m_Text.GetComponent<TextRendererComponent>();
		text.Text = "FPS:" + std::to_string(cnt);
		time -= 1.0f;
		cnt = 0;
	}

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


	m_Scene->OnUpdate(dt); // Renders the scene
}

void Game::OnDestroy()
{
	AssetLibrary::ShutDown();
}

Entity Game::CreateCamera()
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

Entity Game::CreateSprite()
{
	// Create a blue quad
	Entity quad = m_Scene->CreateEntity("Blue quad");

	quad.AddComponent<SpriteRendererComponent>(glm::vec2{ 200.f, 200.f }, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f }); // AddComponent 賦予性質

	auto& transform = quad.GetComponent<TransformComponent>();	//GetComponent 取得性質
	transform.Position = { -300, 300, 0 };

	return quad;
}

Entity Game::CreateSpriteWithTexture(const std::string& path)
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

Entity Game::CreateFloor(int i, int j) {
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
	transform.Position = { i* 96, 0, j * 96 }; // 不會影響物體位置(被UIElementComponent覆蓋)
	transform.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	transform.Rotation = glm::rotate(transform.Rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	return quad;
}

Entity Game::LoadModel()
{
	Entity model = Model::Load(m_Scene, "Model/colorball/colorball.dae"); // 載入模型
	auto& transform = model.GetComponent<TransformComponent>();
	transform.Position.x = 00.f;
	transform.Scale *= 10.1f;

	return model;
}

Entity Game::LoadModelWithTexture()
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

Entity Game::CreateModelByCopying(Entity copySource)
{
	Entity model = m_Scene->CreateEntity(copySource, "Name for newly created model");

	auto& transform = model.GetComponent<TransformComponent>();
	transform.Position.x += 100.f;
	
	return model;
}

Entity Game::CreateText()
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
/*
Entity Game::CreateWarning(int warn)
{
	w = new Warning(m_Scene, warn);
	Entity text = m_Scene->CreateEntity("Text");

	auto& transform = text.GetComponent<TransformComponent>();
	transform.Scale *= 2.f;


	auto& UI = text.AddComponent<UIElementComponent>(); //加UIElementComponent讓物體變成2D的，物體位置由下面三行決定，Transform.Position沒用了
	UI.Anchor = glm::vec2(-1.0f, -1.0f);
	UI.Pivot = glm::vec2(-1.0f, -1.0f);
	UI.Offset = glm::vec2(0.0f, 0.0f);

	auto& textRenderer = text.AddComponent<TextRendererComponent>();

	switch (warn) 
	{
		case 1://tower lvl over 4
		{
			textRenderer.Text = "Lvl 4 Tower Cannot Upgrade!";
			break;
		}
		default:
		{
			textRenderer.Text = "just a test";
			break;
		}
	}

	textRenderer.Font = AssetManager::LoadFont("Fonts/OpenSans-Regular.ttf");
	textRenderer.FontSize = 20.0f;
	textRenderer.Size = glm::vec2(200.0f, 100.0f);
	textRenderer.OutlineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	textRenderer.OutlineThickness = 0.16f; // In the range [0, 0.3f] (not sure)

	return text;
	
}*/

Entity Game::CreateMusic()
{
	Entity music = m_Scene->CreateEntity("Music");
	auto& source = music.AddComponent<MusicSourceComponent>();
	source.Music = AssetManager::LoadMusic("Sound/Free_Test_Data_1MB_MP3.mp3");
	source.Volume = 0.9f;
	source.FadeIn = 2.0f;
	source.FadeOut = 5.0f;

	return music;
}

Entity Game::CreateSound()
{
	Entity sound = m_Scene->CreateEntity("sound");
	auto& source = sound.AddComponent<SoundSourceComponent>();
	source.Sound = AssetManager::LoadSound("Sound/scary-swoosh-142323.mp3");
	source.Volume = 1.0f;
	source.FadeIn = 0.0f;
	source.FadeOut = 0.0f;

	return sound;
}

