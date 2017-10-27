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

bool GamepadInput::ButtonPressed(int ID, GAMEPAD_BUTTON button)
{
	if ((controllers[ID].State.Gamepad.wButtons & button) && !(prevStates[ID].Gamepad.wButtons & button))
		return true;
	else if (button & GAMEPAD_LEFT_TRIGGER || button & GAMEPAD_RIGHT_TRIGGER)
		if (TriggerPressed(ID, (button == GAMEPAD_LEFT_TRIGGER) ? LEFT : RIGHT))
			return true;
	return false;
}

bool GamepadInput::ButtonHeld(int ID, GAMEPAD_BUTTON button)
{
	if (controllers[ID].State.Gamepad.wButtons & button)
		return true;
	else if (button & GAMEPAD_LEFT_TRIGGER || button & GAMEPAD_RIGHT_TRIGGER)
		if (TriggerHeld(ID, (button == GAMEPAD_LEFT_TRIGGER) ? LEFT : RIGHT))
			return true;
	return false;
}

bool GamepadInput::ButtonReleased(int ID, GAMEPAD_BUTTON button)
{
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

float GamepadInput::GetStickX(int ID, SIDE side)
{
	float val = 0.0f;
	float val2 = 0.0f;
	if (side == LEFT)
	{
		val = (controllers[ID].State.Gamepad.sThumbLX) / 32767.0f;
		val2 = (controllers[ID].State.Gamepad.sThumbLY) / 32767.0f;
	}
	else
	{
		val = (controllers[ID].State.Gamepad.sThumbRX) / 32767.0f;
		val2 = (controllers[ID].State.Gamepad.sThumbRY) / 32767.0f;
	}
	return (LengthOfVec2(val, val2) > XINPUT_GAMEPAD_THUMB_DEADZONE) ? val : 0.0f;
}

float GamepadInput::GetStickY(int ID, SIDE side)
{
	float val = 0.0f;
	float val2 = 0.0f;
	if (side == LEFT)
	{
		val = (controllers[ID].State.Gamepad.sThumbLY) / 32767.0f;
		val2 = (controllers[ID].State.Gamepad.sThumbLX) / 32767.0f;
	}
	else
	{
		val = (controllers[ID].State.Gamepad.sThumbRY) / 32767.0f;
		val2 = (controllers[ID].State.Gamepad.sThumbRX) / 32767.0f;
	}
	return (LengthOfVec2(val, val2) > XINPUT_GAMEPAD_THUMB_DEADZONE) ? val : 0.0f;
}

Vector GamepadInput::GetStick(int ID, SIDE side)
{
	return Vector(GetStickX(ID, side), GetStickY(ID, side));
}

bool GamepadInput::IsConnected(int ID)
{
	return controllers[ID].IsConnected;
}