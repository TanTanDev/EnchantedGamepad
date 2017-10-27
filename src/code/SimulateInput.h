#pragma once
#include "Singleton.h"
#include "Vector.h"

class SimulateInput : public Singleton<SimulateInput>
{
public:
	SimulateInput();
	~SimulateInput();

	void PressKey(int key);
	void PressKey(int key1, int key2);
	void KeyDown(int key);
	void KeyUp(int key);
	void ScrollMouse(int delta);
	void SetMousePos(float x, float y);
	void MoveMouse(float x, float y);

	Vector GetMousePos();
	// Called if user moves mouse
	void OnMouseMoved(Vector pos);
	void OnMouseMoved(float x, float y);
private:
	Vector mousePosition;
};