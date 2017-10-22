#include "SimulateInput.h"

#include <Windows.h>
#include <iostream>

// Defined lower down in file
inline void SendInput(int key, bool isDown);
inline LONG getAbsolueScreenCoord(float resolution, float pos);

SimulateInput::SimulateInput()
{
	// todo: Move these values somewhere else, ugly ;)
	screenResolutionHeight = GetSystemMetrics(SM_CYSCREEN);
	screenResolutionWidth = GetSystemMetrics(SM_CXSCREEN);
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
	input.mi.dx = getAbsolueScreenCoord(screenResolutionWidth, x);
	input.mi.dy = getAbsolueScreenCoord(screenResolutionHeight, y);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
}

void SimulateInput::MoveMouse(float x, float y)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
}

inline LONG getAbsolueScreenCoord(float resolution, float pos)
{
	// Absolute position goes from 0 -> 65536.0f that's why we do this
	return pos * (65536.0f / resolution);
}

inline void SendInput(int key, bool isDown)
{
	INPUT input;
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
		if (isDown)
			input.ki.dwFlags = KEYEVENTF_KEYUP;
		// Special keys
		if (key == VK_LEFT || key == VK_RIGHT || key == VK_UP || key == VK_DOWN)
			input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	}
	SendInput(1, &input, sizeof(INPUT));
}