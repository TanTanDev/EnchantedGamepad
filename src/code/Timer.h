// This file is part of Enchanted Gamepad
// Copyright (C) <2018> Jonatan Olsson
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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