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
