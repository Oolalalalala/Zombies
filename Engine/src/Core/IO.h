#pragma once

#include "glm/glm.hpp"

enum class KeyCode
{
	A = 4, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, Num1, Num2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0, Return, Escape, Backspace, Tab, Space, Minus, Equal, LeftBracket, RightBracket, Backslash, Semicolon = 51, Apostrophe, Grave, Comma, Period, Slash, Capslock, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, LeftCtrl = 224, LeftShift, LeftAlt, LeftGUI, RightCtrl, RightShift, RightAlt, RightGUI
};

enum class MouseButton
{
	Left = 1, Middle = 2, Right = 4
};

struct SDL_KeyboardEvent;

class IO
{
public:
	static bool IsKeyDown(KeyCode key);
	static bool IsKeyPressed(KeyCode key);
	static bool IsKeyReleased(KeyCode key);
	static bool IsMouseButtonDown(MouseButton button);
	static bool IsMouseButtonPressed(MouseButton button);
	static bool IsMouseButtonReleased(MouseButton button);
	static glm::vec2 GetMousePosition();
	static float GetMouseScroll();
	static glm::vec2 GetWindowSize();
	static void SetCursorVisibility(bool visible);
	static void SetCursorPosition(glm::vec2 position);

	static bool IsWindowResized();
	static bool IsWindowMinimized();
	
	// Set by SDL
public:
	static void CacheLastFrameKeyboard();
	static void PullData(); // Pull the data from SDL
	static void SetMouseWheel(float scroll);
};