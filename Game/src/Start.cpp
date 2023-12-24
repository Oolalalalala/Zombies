#include "Start.h"
#include "cstdlib"
#include "ctime"
Start::Start()
{
	s_Scene = CreateRef<Scene>("Demo");
	s_Camera = CreateCamera();

	s_PressToPlay = s_Scene->CreateEntity("ptp");

	s_PTP = AssetManager::LoadTexture("start/start_presstoplay.png");
	auto& spriteRenderer = s_PressToPlay.AddComponent<SpriteRendererComponent>(); // AddComponent 賦予性質
	spriteRenderer.Size = glm::vec2(666*4.f, 375*4.f);
	spriteRenderer.Texture = s_PTP;

	auto& UI = s_PressToPlay.AddComponent<UIElementComponent>(); // 加上UIElementComponent把3D物件變成2D，體位置由下面三行決定，Transform.Position沒用了
	UI.Anchor = { 0, 0 };
	UI.Pivot = { 0, 0 };
	UI.Offset = { 0, -10 };


	s_ChenChengPo = s_Scene->CreateEntity("ccp");

	s_CCP = AssetManager::LoadTexture("start/Chen_Chengpo_nd.png");
	auto& spriteRendererr = s_ChenChengPo.AddComponent<SpriteRendererComponent>(); // AddComponent 賦予性質
	spriteRendererr.Size = glm::vec2(300.f, 200.f);
	spriteRendererr.Texture = s_CCP;

	auto& UII = s_ChenChengPo.AddComponent<UIElementComponent>(); // 加上UIElementComponent把3D物件變成2D，體位置由下面三行決定，Transform.Position沒用了
	UII.Anchor = { 0, 0 };
	UII.Pivot = { 0, 0 };
	UII.Offset = { 0, 0 };
	
}
Start::~Start()
{
	AssetManager::UnloadTexture(s_CCP);
	AssetManager::UnloadTexture(s_PTP);
}
void Start::OnUpdate(float dt)
{
	srand(time(NULL));
	int x = rand()*373 % 1000;
	int y = rand()*373 % 1000;
	SetPosition(x, y);
	s_Scene->OnUpdate(dt);
}

void Start::SetStartCallback(std::function<void()> callback)
{
	s_PressToPlay.AddComponent<ButtonComponent>().ButtonDownCallback = callback;
}

void Start::SetPosition(int x, int y)
{
	auto& UI = s_ChenChengPo.GetComponent<UIElementComponent>(); // 加上UIElementComponent把3D物件變成2D，體位置由下面三行決定，Transform.Position沒用了
	UI.Anchor = { 0, 0 };
	UI.Pivot = { 0, 0 };
	UI.Offset = { x, y };
}
Entity Start::CreateCamera()
{
	// Create a camera
	Entity camera = s_Scene->CreateEntity("Camera");
	PerspectiveCameraSpecification specs;
	specs.FarPlane = 2500.f;
	camera.AddComponent<CameraComponent>(Camera(specs));

	auto& transform = camera.GetComponent<TransformComponent>();
	transform.Position = { 0.0f, 0.0f, 100.f };
	transform.Rotation = glm::quat(glm::vec3{ 0.0f, 180.0f, 0.0f });

	return camera;
}