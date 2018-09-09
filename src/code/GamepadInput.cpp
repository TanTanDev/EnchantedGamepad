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

#include "GamepadInput.h"
#include <Windows.h>
#include <XInput.h>
#include <iostream>
#include <limits>

template<class TYPE>
inline TYPE LengthOfVec2(TYPE data1, TYPE data2)
{
	return sqrt(data1*data1 + data2*data2);
}


GamepadInput::GamepadInput()
{
	for (unsigned int i = 0; i < MAX_CONTROLLERS; i++)
	{
		prevStates[i] = controllers[i].State;
		ZeroMemory(&controllers[i], sizeof(XINPUT_STATE));
		ZeroMemory(&prevStates[i], sizeof(XINPUT_STATE));
	}
	_XINPUT_CAPABILITIES des;
	XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &des);
	if (des.Flags & XINPUT_CAPS_WIRELESS)
	{
		std::cout << "WIRELESS CONTROLLER!\n";
	}
}

GamepadInput::~GamepadInput()
{
}

void GamepadInput::Update(float DeltaTime)
{
	for (unsigned int i = 0; i < MAX_CONTROLLERS; i++)
	{
		prevStates[i] = controllers[i].State;
		ZeroMemory(&controllers[i], sizeof(XINPUT_STATE));
		controllers[i].IsConnected = (XInputGetState(0, &controllers[i].State) == ERROR_SUCCESS);

		if (controllers[i].VibrationTimer > std::numeric_limits<float>::epsilon())
		{
			controllers[i].VibrationTimer -= DeltaTime;
			if (controllers[i].VibrationTimer >= 0.0f)
				XInputSetState(i, &controllers[i].Vibration);
		}
	}
}

bool GamepadInput::ButtonPressed(int ID, GAMEPAD_BUTTON button, SIDE joystickSide)
{
	if (button == GAMEPAD_JOYSTICK_LEFT || button == GAMEPAD_JOYSTICK_RIGHT || button == GAMEPAD_JOYSTICK_UP || button == GAMEPAD_JOYSTICK_DOWN)
		return JoystickPressed(ID, joystickSide, button);
	if ((controllers[ID].State.Gamepad.wButtons & button) && !(prevStates[ID].Gamepad.wButtons & button))
		return true;
	else if (button & GAMEPAD_LEFT_TRIGGER || button & GAMEPAD_RIGHT_TRIGGER)
		if (TriggerPressed(ID, (button == GAMEPAD_LEFT_TRIGGER) ? LEFT : RIGHT))
			return true;
	return false;
}

bool GamepadInput::ButtonHeld(int ID, GAMEPAD_BUTTON button, SIDE joystickSide)
{
	if (button == GAMEPAD_JOYSTICK_LEFT || button == GAMEPAD_JOYSTICK_RIGHT || button == GAMEPAD_JOYSTICK_UP || button == GAMEPAD_JOYSTICK_DOWN)
		return JoystickHeld(ID, joystickSide, button);
	if (controllers[ID].State.Gamepad.wButtons & button)
		return true;
	else if (button & GAMEPAD_LEFT_TRIGGER || button & GAMEPAD_RIGHT_TRIGGER)
		if (TriggerHeld(ID, (button == GAMEPAD_LEFT_TRIGGER) ? LEFT : RIGHT))
			return true;
	return false;
}

bool GamepadInput::ButtonReleased(int ID, GAMEPAD_BUTTON button, SIDE joystickSide)
{
	if (button == GAMEPAD_JOYSTICK_LEFT || button == GAMEPAD_JOYSTICK_RIGHT || button == GAMEPAD_JOYSTICK_UP || button == GAMEPAD_JOYSTICK_DOWN)
		return JoystickReleased(ID, joystickSide, button);
	if (!(controllers[ID].State.Gamepad.wButtons & button) && (prevStates[ID].Gamepad.wButtons & button))
		return true;
	else if (button & GAMEPAD_LEFT_TRIGGER || button & GAMEPAD_RIGHT_TRIGGER)
		if (TriggerReleased(ID, (button == GAMEPAD_LEFT_TRIGGER) ? LEFT : RIGHT))
			return true;
	return false;
}

bool GamepadInput::TriggerPressed(int ID, SIDE side)
{
	if (side == LEFT)
		return (controllers[ID].State.Gamepad.bLeftTrigger > 0 && prevStates[ID].Gamepad.bLeftTrigger <= 0);
	else
		return (controllers[ID].State.Gamepad.bRightTrigger > 0 && prevStates[ID].Gamepad.bRightTrigger <= 0);
}

bool GamepadInput::TriggerHeld(int ID, SIDE side)
{
	if (side == LEFT)
		return (controllers[ID].State.Gamepad.bLeftTrigger > 0);
	else
		return (controllers[ID].State.Gamepad.bRightTrigger > 0);
}

bool GamepadInput::TriggerReleased(int ID, SIDE side)
{
	if (side == LEFT)
		return (controllers[ID].State.Gamepad.bLeftTrigger <= 0 && prevStates[ID].Gamepad.bLeftTrigger > 0);
	else
		return (controllers[ID].State.Gamepad.bRightTrigger <= 0 && prevStates[ID].Gamepad.bRightTrigger > 0);
}

//float GamepadInput::GetStickX(int ID, SIDE side, bool getPrevious = false)
//{
//	float val = 0.0f;
//	float val2 = 0.0f;
//	if (side == LEFT)
//	{
//		val = (controllers[ID].State.Gamepad.sThumbLX) / 32767.0f;
//		val2 = (controllers[ID].State.Gamepad.sThumbLY) / 32767.0f;
//	}
//	else
//	{
//		val = (controllers[ID].State.Gamepad.sThumbRX) / 32767.0f;
//		val2 = (controllers[ID].State.Gamepad.sThumbRY) / 32767.0f;
//	}
//	return (LengthOfVec2(val, val2) > XINPUT_GAMEPAD_THUMB_DEADZONE) ? val : 0.0f;
//}
//
//float GamepadInput::GetStickY(int ID, SIDE side, bool getPrevious = false)
//{
//	float val = 0.0f;
//	float val2 = 0.0f;
//	if (side == LEFT)
//	{
//		val = (controllers[ID].State.Gamepad.sThumbLY) / 32767.0f;
//		val2 = (controllers[ID].State.Gamepad.sThumbLX) / 32767.0f;
//	}
//	else
//	{
//		val = (controllers[ID].State.Gamepad.sThumbRY) / 32767.0f;
//		val2 = (controllers[ID].State.Gamepad.sThumbRX) / 32767.0f;
//	}
//	return (LengthOfVec2(val, val2) > XINPUT_GAMEPAD_THUMB_DEADZONE) ? val : 0.0f;
//}

Vector GamepadInput::GetStick(int ID, SIDE side, bool getPrevious, float threshold)
{
	XINPUT_STATE state;
	if (getPrevious)
		state = prevStates[ID];
	else
		state = controllers[ID].State;

	float x = 0.0f;
	float y = 0.0f;
	if (side == LEFT)
	{
		x = state.Gamepad.sThumbLX / 32767.0f;
		y = state.Gamepad.sThumbLY / 32767.0f;
	}
	else
	{
		x = state.Gamepad.sThumbRX / 32767.0f;
		y = state.Gamepad.sThumbRY / 32767.0f;
	}
	return (LengthOfVec2(x, y) > threshold) ? Vector(x, y) : Vector(0.0f, 0.0f);
}

float GamepadInput::GetTrigger(int ID, SIDE side)
{
	if (side == LEFT)
	{
		return controllers[ID].State.Gamepad.bLeftTrigger/255.0f;
	}
	else
	{
		return  controllers[ID].State.Gamepad.bRightTrigger / 255.0f;
	}
	return 0.0f;
}

//Vector GamepadInput::GetStick(int ID, SIDE side, bool getPrevious = false)
//{
//	return Vector(GetStickX(ID, side), GetStickY(ID, side));
//}

bool GamepadInput::JoystickPressed(int ID, SIDE side, GAMEPAD_BUTTON joystickDireciton)
{
	Vector current = GetStick(ID, side);
	Vector prev = GetStick(ID, side, true);
	bool currentPressed = checkAxisByButtonJoystick(current, joystickDireciton);
	bool prevPressed = checkAxisByButtonJoystick(prev, joystickDireciton);
	if (currentPressed && !prevPressed)
		return true;

	return false;
}

bool GamepadInput::JoystickHeld(int ID, SIDE side, GAMEPAD_BUTTON joystickDireciton)
{
	Vector current = GetStick(ID, side);
	bool currentPressed = checkAxisByButtonJoystick(current, joystickDireciton);
	if (currentPressed)
		return true;

	return false;
}

bool GamepadInput::JoystickReleased(int ID, SIDE side, GAMEPAD_BUTTON joystickDireciton)
{
	Vector current = GetStick(ID, side);
	Vector prev = GetStick(ID, side, true);
	bool currentPressed = checkAxisByButtonJoystick(current, joystickDireciton);
	bool prevPressed = checkAxisByButtonJoystick(prev, joystickDireciton);
	if (!currentPressed && prevPressed)
		return true;

	return false;
}

void GamepadInput::SetVibration(float strength, SIDE side, float time)
{
	if(side == SIDE::LEFT)
		controllers[0].Vibration.wLeftMotorSpeed = strength*65535;
	if (side == SIDE::RIGHT)
		controllers[0].Vibration.wRightMotorSpeed = strength*65535;
	controllers[0].VibrationTimer = time;
}

bool GamepadInput::checkAxisByButtonJoystick(Vector axis, GAMEPAD_BUTTON joystickAxis)
{
	if (joystickAxis == GAMEPAD_JOYSTICK_LEFT)
		return (axis.x) < -XINPUT_GAMEPAD_THUMB_DEADZONE;
	else if (joystickAxis == GAMEPAD_JOYSTICK_RIGHT)
		return (axis.x) > XINPUT_GAMEPAD_THUMB_DEADZONE;
	else if (joystickAxis == GAMEPAD_JOYSTICK_UP)
		return (axis.y) > XINPUT_GAMEPAD_THUMB_DEADZONE;
	else if (joystickAxis == GAMEPAD_JOYSTICK_DOWN)
		return (axis.y) < -XINPUT_GAMEPAD_THUMB_DEADZONE;
	return 0.0f;
}

bool GamepadInput::IsConnected(int ID)
{
	return controllers[ID].IsConnected;
}