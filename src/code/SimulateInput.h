#pragma once
#include "Singleton.h"

class SimulateInput : public Singleton<SimulateInput>
{
private:
	int screenResolutionWidth;
	int screenResolutionHeight;
public:
	SimulateInput();
	~SimulateInput();

	void PressKey(int keyssssss);
	void KeyDown(int key);
	void KeyUp(int key);
	void ScrollMouse(int delta);
	void SetMousePos(float x, float y);
	void MoveMouse(float x, float y);
};