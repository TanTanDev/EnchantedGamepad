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

#include "SimulateInput.h"
#include "AppSettings.h"
#include "Vector.h"

#include <Windows.h>
#include <iostream>

// Declerations
inline void SendInput(int key, bool isDown);
inline LONG getAbsoluteScreenCoord(float resolution, float pos);

SimulateInput::SimulateInput()
{
//	mousePosition = Vector();
}

SimulateInput::~SimulateInput()
{
	std::cout << "BYE" << std::endl;
}

void SimulateInput::PressKey(int key = 0x51)
{
	SendInput(key, true);
	SendInput(key, false);
}

void SimulateInput::PressKey(int key1, int key2)
{
	SendInput(key1, true);
	if (key2 != 0)
		SendInput(key2, true);

	SendInput(key1, false);
	if (key2 != 0)
		SendInput(key2, false);
}

void SimulateInput::KeyDown(int key)
{
	SendInput(key, true);
}

void SimulateInput::KeyUp(int key)
{
	SendInput(key, false);
}

void SimulateInput::ScrollMouse(int delta)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	input.mi.mouseData = delta;
	SendInput(1, &input, sizeof(INPUT));
}

void SimulateInput::SetMousePos(float x, float y)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = 0.5f + getAbsoluteScreenCoord(AppSettings::GetInstance().screenResolutionWidth, x);
	input.mi.dy = 0.5f + getAbsoluteScreenCoord(AppSettings::GetInstance().screenResolutionHeight, y);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));

	OnMouseMoved(x, y);
}

void SimulateInput::MoveMouse(float x, float y)
{
	// if we use delta based values this won't work
	//INPUT input;
	//input.type = INPUT_MOUSE;
	//input.mi.dx = 0.5f + x;
	//input.mi.dy = 0.5f + y;
	//input.mi.dwFlags = MOUSEEVENTF_MOVE;
	//input.mi.time = 0;
	//SendInput(1, &input, sizeof(INPUT));

	// ensures precision
	mousePosition.x += x;
	mousePosition.y += y;
	SetMousePos(mousePosition.x, mousePosition.y);
}

Vector SimulateInput::GetMousePos()
{
	POINT p;
	if (GetCursorPos(&p))
	{

	}
	return Vector(p.x, p.y);
	//return mousePosition;
}

void SimulateInput::OnMouseMoved(Vector pos)
{
	mousePosition.x = pos.x;
	mousePosition.y = pos.y;
}

void SimulateInput::OnMouseMoved(float x, float y)
{
	mousePosition.x = x;
	mousePosition.y = y;
}

inline LONG getAbsoluteScreenCoord(float resolution, float pos)
{
	// Absolute position goes from 0 -> 65536.0f that's why we do this
	return pos * (65536.0f / resolution);
}

inline void SendInput(int key, bool isDown)
{
	INPUT input;
	//memset(&input, 0, sizeof(INPUT));
	input.ki.dwFlags = 0;
	input.ki.time = 0;
	input.ki.dwFlags = 0;
	input.ki.wVk = key;

	bool isMouseEvent = key == VK_LBUTTON || key == VK_RBUTTON;
	// mouse press
	if (isMouseEvent)
	{
		input.type = INPUT_MOUSE;
		if (isDown)
			input.mi.dwFlags = (key & VK_LBUTTON) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
		else
			input.mi.dwFlags = (key & VK_LBUTTON) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;
		input.ki.wVk = 0;
	}
	else
	{
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = static_cast<WORD>(MapVirtualKeyW(key, MAPVK_VK_TO_VSC));
		if (!isDown)
			input.ki.dwFlags = KEYEVENTF_KEYUP;
		// Special keys
		if (key == VK_LEFT || key == VK_RIGHT || key == VK_UP || key == VK_DOWN)
			input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	}
	SendInput(1, &input, sizeof(INPUT));
}