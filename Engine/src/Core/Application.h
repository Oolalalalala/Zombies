#pragma once
#include "Base.h"
#include "Window.h"

class Application
{
public:
	Application();
	virtual ~Application();

	static Application& Get() { return *s_Instance; }

	void Run();
	void Close();

	virtual void OnCreate() {};
	virtual void OnUpdate(float dt) {};
	virtual void OnDestroy() {};

	Ref<Window> GetWindow() { return m_Window; };

private:
	bool m_Running = true;
	Ref<Window> m_Window;

private:
	// Singleton
	static Application* s_Instance;
public:
};

extern Application* CreateApplication();