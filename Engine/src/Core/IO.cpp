#include "pch.h"
#include "IO.h"

#include "SDL.h"
#include "Application.h"

struct IOData
{
    uint8_t PreviousKeyboardState[SDL_NUM_SCANCODES];
    const uint8_t* KeyboardState;
    int KeyCount;
    int MousePosX, MousePosY;
    float MouseScroll = 0.0f;
    uint32_t PrevMouseState = 0, MouseState = 0;
    int WindowWidth = 0, WindowHeight = 0;
    bool IsWindowResized = false, IsWindowMinimized = false;
};

static IOData s_Data;

bool IO::IsKeyDown(KeyCode key)
{
    if ((int)key >= s_Data.KeyCount) 
        return false;

    return !s_Data.PreviousKeyboardState[(int)key] && s_Data.KeyboardState[(int)key];
}

bool IO::IsKeyPressed(KeyCode key)
{
    if ((int)key >= s_Data.KeyCount)
        return false;
    return s_Data.KeyboardState[(int)key];
}

bool IO::IsKeyReleased(KeyCode key)
{
    if ((int)key >= s_Data.KeyCount) 
        return false;
    
    return s_Data.PreviousKeyboardState[(int)key] && !s_Data.KeyboardState[(int)key];
}

bool IO::IsMouseButtonDown(MouseButton button)
{
    return !(s_Data.PrevMouseState & (int)button) && (s_Data.MouseState & (int)button);
}

bool IO::IsMouseButtonPressed(MouseButton button)
{
    return s_Data.MouseState & (int)button;
}

bool IO::IsMouseButtonReleased(MouseButton button)
{
    return (s_Data.PrevMouseState & (int)button) && !(s_Data.MouseState & (int)button);
}

glm::vec2 IO::GetMousePosition()
{
    return { s_Data.MousePosX, s_Data.MousePosY };
}

float IO::GetMouseScroll()
{
    return s_Data.MouseScroll;
}

glm::vec2 IO::GetWindowSize()
{
    return { s_Data.WindowWidth, s_Data.WindowHeight };
}

void IO::SetCursorVisibility(bool visible)
{
    SDL_ShowCursor(visible);
}

void IO::SetCursorPosition(glm::vec2 position)
{
    SDL_WarpMouseInWindow(Application::Get().GetWindow()->GetNativeWindow(), position.x, position.y);
}

bool IO::IsWindowResized()
{
    return s_Data.IsWindowResized;
}

bool IO::IsWindowMinimized()
{
    return s_Data.IsWindowMinimized;
}

void IO::CacheLastFrameKeyboard()
{
    memcpy(s_Data.PreviousKeyboardState, s_Data.KeyboardState, s_Data.KeyCount);
    s_Data.PrevMouseState = s_Data.MouseState;

    // TODO : move or rename function
    s_Data.MouseScroll = 0.0f;
}

void IO::PullData()
{
    s_Data.KeyboardState = SDL_GetKeyboardState(&s_Data.KeyCount);
    s_Data.MouseState = SDL_GetMouseState(&s_Data.MousePosX, &s_Data.MousePosY);

    int w, h;
    SDL_GetWindowSize(Application::Get().GetWindow()->GetNativeWindow(), &w, &h);

    if (w != s_Data.WindowWidth || h != s_Data.WindowHeight)
    {
        s_Data.IsWindowResized = true;

        s_Data.WindowWidth = w;
        s_Data.WindowHeight = h;
    }
    else
    {
        s_Data.IsWindowResized = false;
    }

    uint32_t flags = SDL_GetWindowFlags(Application::Get().GetWindow()->GetNativeWindow());
    s_Data.IsWindowMinimized = flags & SDL_WINDOW_MINIMIZED;
        
}

void IO::SetMouseWheel(float scroll)
{
    s_Data.MouseScroll = scroll;
}
