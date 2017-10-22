#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;

class Timer
{
public:
	Timer();
	~Timer();
	void Update();
	float GetDeltaTime();
	float GetElapsedTime();
	void Start();
	void Stop();
	void Reset();

private:
	float deltaTime;
	bool isPlaying;
	high_resolution_clock::time_point startTime;
	high_resolution_clock::time_point prevFrameTime;
};