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

#include "InputBinding.h"
#include "GamepadInput.h"

// Needed to push vectors
#include "VectorBinding.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace ScriptBindings
{
	const char tableName[] = "Input";
	inline static void PushInteger(lua_State* L, int v, const char* n);
	inline static void PushMemberVariables(lua_State* L);

	static int ButtonPressed(lua_State* L)
	{
		lua_Integer input = luaL_checkinteger(L, 1);
		lua_Integer joystickSide = 0;
		if(lua_isnumber(L, 2))
			joystickSide = luaL_checkinteger(L, 2);
		int status = GamepadInput::GetInstance().ButtonPressed(0, (GamepadInput::GAMEPAD_BUTTON)input, (GamepadInput::SIDE)joystickSide);
		lua_pushboolean(L, status);
		return 1;
	}

	static int ButtonHeld(lua_State* L)
	{
		lua_Integer input = luaL_checkinteger(L, 1);
		lua_Integer joystickSide = 0;
		if (lua_isnumber(L, 2))
			joystickSide = luaL_checkinteger(L, 2);
		int status = GamepadInput::GetInstance().ButtonHeld(0, (GamepadInput::GAMEPAD_BUTTON)input, (GamepadInput::SIDE)joystickSide);
		lua_pushboolean(L, status);
		return 1;
	}

	static int ButtonReleased(lua_State* L)
	{
		lua_Integer input = luaL_checkinteger(L, 1);
		lua_Integer joystickSide = 0;
		if (lua_isnumber(L, 2))
			joystickSide = luaL_checkinteger(L, 2);
		int status = GamepadInput::GetInstance().ButtonReleased(0, (GamepadInput::GAMEPAD_BUTTON)input, (GamepadInput::SIDE)joystickSide);
		lua_pushboolean(L, status);
		return 1;
	}

	// helper function for joystick to reduce code duplication
	Vector GetStickThreshold(lua_State* L, GamepadInput::SIDE side, int thresholdArgIndex = 1)
	{
		if (lua_isnumber(L, thresholdArgIndex))
		{
			float threshold = (float)lua_tonumber(L, 1);
			return GamepadInput::GetInstance().GetStick(0, side, false, threshold);
		}
		else // default threshold
			GamepadInput::GetInstance().GetStick(0, side);
	}

	static int GetStickLX(lua_State* L)
	{
		float stickAxisValue = GetStickThreshold(L, GamepadInput::LEFT).x;
		lua_pushnumber(L, stickAxisValue);
		return 1;
	}
	
	static int GetStickRX(lua_State* L)
	{
		float stickAxisValue = GetStickThreshold(L, GamepadInput::RIGHT).x;
		lua_pushnumber(L, stickAxisValue);
		return 1;
	}

	static int GetStickLY(lua_State* L)
	{
		float stickAxisValue = GetStickThreshold(L, GamepadInput::LEFT).y;
		lua_pushnumber(L, stickAxisValue);
		return 1;
	}

	static int GetStickRY(lua_State* L)
	{
		float stickAxisValue = GetStickThreshold(L, GamepadInput::RIGHT).x;
		lua_pushnumber(L, stickAxisValue);
		return 1;
	}

	static int GetStick(lua_State* L)
	{
		//GamepadInput::GetInstance().GetStick(0,);
		lua_Integer side = luaL_checkinteger(L, 1);
		Vector stick;
		if (lua_isnumber(L, 2))
		{
			float threshold = luaL_checknumber(L, 2);
			stick = GamepadInput::GetInstance().GetStick(0, (GamepadInput::SIDE)side, false, threshold);
		}
		else
			stick = GamepadInput::GetInstance().GetStick(0, (GamepadInput::SIDE)side);
		lua_pushvector(L, stick.x, stick.y);
		//lua_pushnumber(L, GamepadInput::GetInstance().GetStickY(0, (GamepadInput::SIDE)side));
		return 1;
	}

	// todo: find a way to register with this structure
	static const luaL_Reg Input_Functions[] =
	{
		{ "ButtonPressed", ButtonPressed },
		{ "ButtonHeld", ButtonHeld },
		{ "ButtonReleased", ButtonReleased },
		//{ "GetStickLX", GetStickLX },
		//{ "GetStickRX", GetStickRX },
		//{ "GetStickLY", GetStickLY },
		//{ "GetStickRY", GetStickRY },
		{ NULL, NULL },
	};

	inline static void PushFunctions(lua_State* L)
	{
		lua_pushcfunction(L, ButtonPressed);
		lua_setfield(L, -2, "ButtonPressed");
		lua_pushcfunction(L, ButtonHeld);
		lua_setfield(L, -2, "ButtonHeld");
		lua_pushcfunction(L, ButtonReleased);
		lua_setfield(L, -2, "ButtonReleased");
		lua_pushcfunction(L, GetStickLX);
		lua_setfield(L, -2, "GetStickLX");
		lua_pushcfunction(L, GetStickRX);
		lua_setfield(L, -2, "GetStickRX");
		lua_pushcfunction(L, GetStickLY);
		lua_setfield(L, -2, "GetStickLY");
		lua_pushcfunction(L, GetStickRY);
		lua_setfield(L, -2, "GetStickRY");
		lua_pushcfunction(L, GetStick);
		lua_setfield(L, -2, "GetStick");
	}

	int luaopen_input(lua_State* L)
	{
		lua_newtable(L);
		PushMemberVariables(L);
		PushFunctions(L);
		lua_setglobal(L, tableName);
		return 1;
	}

	inline static void PushInteger(lua_State* L, int v, const char* n)
	{
		lua_pushinteger(L, v);
		lua_setfield(L, -2, n);
	}

	inline static void PushMemberVariables(lua_State* L)
	{
		PushInteger(L, GamepadInput::GAMEPAD_DPAD_UP, "GAMEPAD_DPAD_UP");
		PushInteger(L, GamepadInput::GAMEPAD_DPAD_DOWN, "GAMEPAD_DPAD_DOWN");
		PushInteger(L, GamepadInput::GAMEPAD_DPAD_LEFT, "GAMEPAD_DPAD_LEFT");
		PushInteger(L, GamepadInput::GAMEPAD_DPAD_RIGHT, "GAMEPAD_DPAD_RIGHT");
		PushInteger(L, GamepadInput::GAMEPAD_START, "GAMEPAD_START");
		PushInteger(L, GamepadInput::GAMEPAD_BACK, "GAMEPAD_BACK");
		PushInteger(L, GamepadInput::GAMEPAD_LEFT_THUMB, "GAMEPAD_LEFT_THUMB");
		PushInteger(L, GamepadInput::GAMEPAD_RIGHT_THUMB, "GAMEPAD_RIGHT_THUMB");
		PushInteger(L, GamepadInput::GAMEPAD_LEFT_SHOULDER, "GAMEPAD_LEFT_BUTTON");
		PushInteger(L, GamepadInput::GAMEPAD_RIGHT_SHOULDER, "GAMEPAD_RIGHT_BUTTON");
		PushInteger(L, GamepadInput::GAMEPAD_A, "GAMEPAD_A");
		PushInteger(L, GamepadInput::GAMEPAD_B, "GAMEPAD_B");
		PushInteger(L, GamepadInput::GAMEPAD_X, "GAMEPAD_X");
		PushInteger(L, GamepadInput::GAMEPAD_Y, "GAMEPAD_Y");
		PushInteger(L, GamepadInput::GAMEPAD_LEFT_TRIGGER, "GAMEPAD_LEFT_TRIGGER");
		PushInteger(L, GamepadInput::GAMEPAD_RIGHT_TRIGGER, "GAMEPAD_RIGHT_TRIGGER");
		PushInteger(L, GamepadInput::GAMEPAD_JOYSTICK_LEFT, "GAMEPAD_JOYSTICK_LEFT");
		PushInteger(L, GamepadInput::GAMEPAD_JOYSTICK_RIGHT, "GAMEPAD_JOYSTICK_RIGHT");
		PushInteger(L, GamepadInput::GAMEPAD_JOYSTICK_UP, "GAMEPAD_JOYSTICK_UP");
		PushInteger(L, GamepadInput::GAMEPAD_JOYSTICK_DOWN, "GAMEPAD_JOYSTICK_DOWN");

		// only used to determine what side to fetch joystick data from
		PushInteger(L, GamepadInput::SIDE::LEFT, "GAMEPAD_LEFT_STICK");
		PushInteger(L, GamepadInput::SIDE::RIGHT, "GAMEPAD_RIGHT_STICK");
	}
}
