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