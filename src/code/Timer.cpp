#include "Timer.h"

Timer::Timer()
	:isPlaying(false)
{
	startTime = high_resolution_clock::now();
}

Timer::~Timer()
{
}

float Timer::GetDeltaTime()
{
	return deltaTime;
}

float Timer::GetElapsedTime()
{
	return duration_cast<duration<float>>(prevFrameTime - startTime).count();
}

void Timer::Start()
{
	prevFrameTime = high_resolution_clock::now();
	isPlaying = true;
}

void Timer::Stop()
{
	isPlaying = false;
}

void Timer::Reset()
{
	startTime = high_resolution_clock::now();
	prevFrameTime = high_resolution_clock::now();
}

void Timer::Update()
{
	if (isPlaying)
	{
		auto CurrentFrame = high_resolution_clock::now();
		deltaTime = duration_cast<duration<float>>(CurrentFrame - prevFrameTime).count();
		prevFrameTime = CurrentFrame;
	}
}
