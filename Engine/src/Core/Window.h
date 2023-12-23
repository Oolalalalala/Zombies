#pragma once

#include <string>
#include <functional>

struct WindowSpecification
{
	std::string Title = "New Window";
	uint32_t Width = 1280, Height = 720;
	bool VSync = true;
};

// Forward declaration
struct SDL_Window;

class Window
{
public:
	typedef std::function<void()> EventCallbackFn;

	Window(const WindowSpecification& specification);
	~Window();

	void OnUpdate();
	void SetEventCallback(EventCallbackFn callback) { m_Data.EventCallback = callback; }

	SDL_Window* GetNativeWindow() { return m_NativeWindow; }

private:
	void Initialize(const WindowSpecification& specification);
	void ShutDown();

	struct WindowData 
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};
	WindowData m_Data;

private:
	SDL_Window* m_NativeWindow;
};