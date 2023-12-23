#include "pch.h"
#include "Timer.h"


Timer::Timer()
	: m_LastFrameTime(std::chrono::steady_clock::now())
{

}

float Timer::Tick()
{
	auto currentTime = std::chrono::steady_clock::now();
	auto interval = currentTime - m_LastFrameTime;
	m_LastFrameTime = currentTime;
	return interval.count() * 0.000000001f;
}