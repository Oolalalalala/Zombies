#pragma once

#include <chrono>


class Timer
{
public:
	Timer();

	float Tick(); //In seconds

private:
	std::chrono::time_point<std::chrono::steady_clock> m_LastFrameTime;
};
