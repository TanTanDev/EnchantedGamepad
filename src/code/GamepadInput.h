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
#include <Windows.h>
#include <XInput.h>
#include "Vector.h"

#define MAX_CONTROLLERS 4
#define XINPUT_GAMEPAD_THUMB_DEADZONE 0.2f

class GamepadInput : public Singleton<GamepadInput>
{
public:
	enum GAMEPAD_BUTTON
	{
		GAMEPAD_DPAD_UP = 0x0001,
		GAMEPAD_DPAD_DOWN = 0x0002,
		GAMEPAD_DPAD_LEFT = 0x0004,
		GAMEPAD_DPAD_RIGHT = 0x0008,
		GAMEPAD_START = 0x0010,
		GAMEPAD_BACK = 0x0020,
		GAMEPAD_LEFT_THUMB = 0x0040,
		GAMEPAD_RIGHT_THUMB = 0x0080,
		GAMEPAD_LEFT_SHOULDER = 0x0100,
		GAMEPAD_RIGHT_SHOULDER = 0x0200,
		GAMEPAD_A = 0x1000,
		GAMEPAD_B = 0x2000,
		GAMEPAD_X = 0x4000,
		GAMEPAD_Y = 0x8000,
		// custom
		GAMEPAD_LEFT_TRIGGER = 0x10000,
		GAMEPAD_RIGHT_TRIGGER = 0x20000,

		GAMEPAD_JOYSTICK_LEFT = 0x40000,
		GAMEPAD_JOYSTICK_RIGHT = 0x80000,
		GAMEPAD_JOYSTICK_UP = 0x100000,
		GAMEPAD_JOYSTICK_DOWN = 0x200000
	};
	enum SIDE
	{
		LEFT,
		RIGHT
	};
	GamepadInput();
	~GamepadInput();
	// SIDE is optional
	bool ButtonPressed(int ID, GAMEPAD_BUTTON button, SIDE joystickSide = SIDE::LEFT);
	bool ButtonHeld(int ID, GAMEPAD_BUTTON button, SIDE joystickSide = SIDE::LEFT);
	bool ButtonReleased(int ID, GAMEPAD_BUTTON button, SIDE joystickSide = SIDE::LEFT);

	bool TriggerPressed(int ID, SIDE side);
	bool TriggerHeld(int ID, SIDE side);
	bool TriggerReleased(int ID, SIDE side);

	//float GetStickX(int ID, SIDE side, bool getPrevious);
	//float GetStickY(int ID, SIDE side, bool getPrevious);
	Vector GetStick(int ID, SIDE side, bool getPrevious = false, float threshold = XINPUT_GAMEPAD_THUMB_DEADZONE);

	bool JoystickPressed(int ID, SIDE side, GAMEPAD_BUTTON joystickDireciton);
	bool JoystickHeld(int ID, SIDE side, GAMEPAD_BUTTON joystickDireciton);
	bool JoystickReleased(int ID, SIDE side, GAMEPAD_BUTTON joystickDireciton);

	void SetVibration(float strength, SIDE side, float time);

	// if gamepad button joystick points same as axis
	bool checkAxisByButtonJoystick(Vector axis, GAMEPAD_BUTTON button);

	bool IsConnected(int ID);
	void Update(float DeltaTime);

	struct Controller
	{
		XINPUT_STATE State;
		bool IsConnected;
		XINPUT_VIBRATION Vibration;
		float VibrationTimer;
	};

private:
	Controller controllers[MAX_CONTROLLERS];
	XINPUT_STATE prevStates[MAX_CONTROLLERS];

	const float deadZoneThreashold = 0.2f;//XINPUT_GAMEPAD_THUMB_DEADZONE
};