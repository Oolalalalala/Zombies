#include "pch.h"
#include "Application.h"

#include "Log.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RendererAPI.h"
#include "Audio/AudioMixer.h"
#include "Timer.h"


Application* Application::s_Instance = nullptr;

Application::Application()
{
	s_Instance = this;

	Log::Initialize();
	
	m_Window = CreateRef<Window>(WindowSpecification{"Zombies", 1280, 720, true});

	RendererAPI::Initialize();
	Renderer2D::Initialize();

	AudioMixer::Initialize();
}

Application::~Application()
{
	Renderer2D::ShutDown();

	AudioMixer::ShutDown();
}

void Application::Run()
{
	OnCreate();

	Timer timer;

	while (m_Running)
	{
		OnUpdate(timer.Tick());

		m_Window->OnUpdate();
	}

	OnDestroy();
}

void Application::Close()
{
	m_Running = false;
}
