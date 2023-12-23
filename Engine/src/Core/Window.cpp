#include "pch.h"
#include "Window.h"


#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Application.h"
#include "IO.h"


Window::Window(const WindowSpecification& specification)
{
	Initialize(specification);
}

Window::~Window()
{
	ShutDown();
}

void Window::Initialize(const WindowSpecification& specification)
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
		CORE_CRITICAL("Failed to initialize SDL_VIEDO");

	m_NativeWindow = SDL_CreateWindow(specification.Title.c_str(), 100, 100, specification.Width, specification.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!m_NativeWindow)
		CORE_CRITICAL("Failed to create window");
	
	SDL_GL_CreateContext(m_NativeWindow);

	if (glewInit() != GLEW_OK)
		CORE_CRITICAL("Failed to initialize GLEW");

}

void Window::ShutDown()
{
	SDL_DestroyWindow(m_NativeWindow);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Window::OnUpdate()
{
	IO::CacheLastFrameKeyboard();

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			Application::Get().Close();
			break;
		}

	}
	SDL_GL_SwapWindow(m_NativeWindow);

	IO::PullData();
}
