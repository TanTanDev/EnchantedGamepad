#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <XInput.h>
#include "Vector.h"

static const int MAX_CONTROLLERS = 4;
static const float XINPUT_GAMEPAD_THUMB_DEADZONE = 0.2f;

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
		// Custom
		GAMEPAD_LEFT_TRIGGER = 0x10000,
		GAMEPAD_RIGHT_TRIGGER = 0x20000,
	};
	enum SIDE
	{
		LEFT,
		RIGHT
	};
	GamepadInput();
	~GamepadInput();
	bool ButtonPressed(int ID, GAMEPAD_BUTTON button);
	bool ButtonHeld(int ID, GAMEPAD_BUTTON button);
	bool ButtonReleased(int ID, GAMEPAD_BUTTON button);

	bool TriggerPressed(int ID, SIDE side);
	bool TriggerHeld(int ID, SIDE side);
	bool TriggerReleased(int ID, SIDE side);

	float GetStickX(int ID, SIDE side);
	float GetStickY(int ID, SIDE side);
	Vector GetStick(int ID, SIDE side);

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
};