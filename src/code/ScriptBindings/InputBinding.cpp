#include "InputBinding.h"
#include "GamepadInput.h"

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
		int status = GamepadInput::GetInstance().ButtonPressed(0, (GamepadInput::GAMEPAD_BUTTON)input);
		lua_pushboolean(L, status);
		return 1;
	}

	static int ButtonHeld(lua_State* L)
	{
		lua_Integer input = luaL_checkinteger(L, 1);
		int status = GamepadInput::GetInstance().ButtonHeld(0, (GamepadInput::GAMEPAD_BUTTON)input);
		lua_pushboolean(L, status);
		return 1;
	}

	static int ButtonReleased(lua_State* L)
	{
		lua_Integer input = luaL_checkinteger(L, 1);
		int status = GamepadInput::GetInstance().ButtonReleased(0, (GamepadInput::GAMEPAD_BUTTON)input);
		lua_pushboolean(L, status);
		return 1;
	}

	static int GetStickLX(lua_State* L)
	{
		lua_pushnumber(L, GamepadInput::GetInstance().GetStickX(0, GamepadInput::LEFT));
		return 1;
	}

	static int GetStickRX(lua_State* L)
	{
		lua_pushnumber(L, GamepadInput::GetInstance().GetStickX(0, GamepadInput::RIGHT));
		return 1;
	}

	static int GetStickLY(lua_State* L)
	{
		lua_pushnumber(L, GamepadInput::GetInstance().GetStickY(0, GamepadInput::LEFT));
		return 1;
	}

	static int GetStickRY(lua_State* L)
	{
		lua_pushnumber(L, GamepadInput::GetInstance().GetStickY(0, GamepadInput::RIGHT));
		return 1;
	}
	// todo: find a way to register with this structure
	static const luaL_Reg Input_Functions[] =
	{
		{ "ButtonPressed", ButtonPressed },
		{ "ButtonHeld", ButtonHeld },
		{ "ButtonReleased", ButtonReleased },
		{ "GetStickLX", GetStickLX },
		{ "GetStickRX", GetStickRX },
		{ "GetStickLY", GetStickLY },
		{ "GetStickRY", GetStickRY },
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
	}
}
