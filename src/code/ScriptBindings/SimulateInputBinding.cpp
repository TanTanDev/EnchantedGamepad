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

#include "SimulateInputBinding.h"
#include "SimulateInput.h"

// Needed to read data from vectors
#include "VectorBinding.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace ScriptBindings
{
	const char tableName[] = "SimulateInput";
	inline static void PushInteger(lua_State* L, int v, const char* n);
	inline static void PushFunctions(lua_State* L);
	inline static void PushMemberVariables(lua_State* L);

	/*LUALIB_API*/ int luaopen_simulateinput(lua_State* L)
	{
		lua_newtable(L);
		PushMemberVariables(L);
		PushFunctions(L);
		lua_setglobal(L, tableName);
		return 1;
	}

	static int PressKey(lua_State* L)
	{
		lua_Integer key = luaL_checkinteger(L, 1);
		lua_Integer key2 = 0;
		if (lua_isnumber(L, 2))
			key2 = luaL_checkinteger(L, 2);
		SimulateInput::GetInstance().PressKey(key, key2);
		return 0;
	}

	static int KeyDown(lua_State* L)
	{
		lua_Integer key = luaL_checkinteger(L, 1);
		SimulateInput::GetInstance().KeyDown(key);
		return 0;
	}


	static int KeyUp(lua_State* L)
	{
		lua_Integer key = luaL_checkinteger(L, 1);
		SimulateInput::GetInstance().KeyUp(key);
		return 0;
	}

	static int SetMousePos(lua_State* L)
	{
		Vector vector = lua_checkvectorEx(L, 1);
		SimulateInput::GetInstance().SetMousePos(vector.x, vector.y);
		return 0;
	}

	static int MoveMouse(lua_State* L)
	{
		Vector vector = lua_checkvectorEx(L, 1);
		SimulateInput::GetInstance().MoveMouse(vector.x, vector.y);
		return 0;
	}

	static int ScrollMouse(lua_State* L)
	{
		float delta = luaL_checknumber(L, 1);
		SimulateInput::GetInstance().ScrollMouse(delta);
		return 0;
	}

	static int GetMousePos(lua_State* L)
	{
		//float delta = luaL_checknumber(L, 1);
		Vector mousePos = SimulateInput::GetInstance().GetMousePos();
		lua_pushvector(L, mousePos.x, mousePos.y);
		return 1;
	}

	inline static void PushFunctions(lua_State* L)
	{
		lua_pushcfunction(L, KeyUp);
		lua_setfield(L, -2, "KeyUp");
		lua_pushcfunction(L, KeyDown);
		lua_setfield(L, -2, "KeyDown");
		lua_pushcfunction(L, PressKey);
		lua_setfield(L, -2, "PressKey");
		lua_pushcfunction(L, SetMousePos);
		lua_setfield(L, -2, "SetMousePos");
		lua_pushcfunction(L, MoveMouse);
		lua_setfield(L, -2, "MoveMouse");
		lua_pushcfunction(L, ScrollMouse);
		lua_setfield(L, -2, "ScrollMouse");
		lua_pushcfunction(L, GetMousePos);
		lua_setfield(L, -2, "GetMousePos");
	}

	inline static void PushInteger(lua_State* L, int v, const char* n)
	{
		lua_pushinteger(L, v);
		lua_setfield(L, -2, n);
	}

	inline static void PushMemberVariables(lua_State* L)
	{
		PushInteger(L, 0x01, "MOUSE_LEFT");
		PushInteger(L, 0x02, "MOUSE_RIGHT");
		PushInteger(L, 0x03, "KEY_CANCEL");
		PushInteger(L, 0x04, "KEY_MBUTTON");
		PushInteger(L, 0x05, "KEY_XBUTTON1");
		PushInteger(L, 0x06, "KEY_XBUTTON2");
		PushInteger(L, 0x08, "KEY_BACK");
		PushInteger(L, 0x09, "KEY_TAB");
		PushInteger(L, 0x0C, "KEY_CLEAR");
		PushInteger(L, 0x0D, "KEY_RETURN");
		PushInteger(L, 0x10, "KEY_SHIFT");
		PushInteger(L, 0x11, "KEY_CTRL");
		PushInteger(L, 0x12, "KEY_MENU");
		PushInteger(L, 0x13, "KEY_PAUSE");
		PushInteger(L, 0x14, "KEY_CAPITAL");
		PushInteger(L, 0x15, "KEY_KANA");
		PushInteger(L, 0x15, "KEY_HANGUEL");
		PushInteger(L, 0x15, "KEY_HANGUL");
		PushInteger(L, 0x17, "KEY_JUNJA");
		PushInteger(L, 0x18, "KEY_FINAL");
		PushInteger(L, 0x19, "KEY_HANJA");
		PushInteger(L, 0x19, "KEY_KANJI");
		PushInteger(L, 0x1B, "KEY_ESCAPE");
		PushInteger(L, 0x1C, "KEY_CONVERT");
		PushInteger(L, 0x1D, "KEY_NONCONVERT");
		PushInteger(L, 0x1E, "KEY_ACCEPT");
		PushInteger(L, 0x1F, "KEY_MODECHANGE");
		PushInteger(L, 0x20, "KEY_SPACE");
		PushInteger(L, 0x21, "KEY_PRIOR");
		PushInteger(L, 0x22, "KEY_NEXT");
		PushInteger(L, 0x23, "KEY_END");
		PushInteger(L, 0x24, "KEY_HOME");
		PushInteger(L, 0x25, "KEY_LEFT");
		PushInteger(L, 0x26, "KEY_UP");
		PushInteger(L, 0x27, "KEY_RIGHT");
		PushInteger(L, 0x28, "KEY_DOWN");
		PushInteger(L, 0x29, "KEY_SELECT");
		PushInteger(L, 0x2A, "KEY_PRINT");
		PushInteger(L, 0x2B, "KEY_EXECUTE");
		PushInteger(L, 0x2C, "KEY_SNAPSHOT");
		PushInteger(L, 0x2D, "KEY_INSERT");
		PushInteger(L, 0x2E, "KEY_DELETE");
		PushInteger(L, 0x2F, "KEY_HELP");
		PushInteger(L, 0x30, "KEY_0");
		PushInteger(L, 0x31, "KEY_1");
		PushInteger(L, 0x32, "KEY_2");
		PushInteger(L, 0x33, "KEY_3");
		PushInteger(L, 0x34, "KEY_4");
		PushInteger(L, 0x35, "KEY_5");
		PushInteger(L, 0x36, "KEY_6");
		PushInteger(L, 0x37, "KEY_7");
		PushInteger(L, 0x38, "KEY_8");
		PushInteger(L, 0x39, "KEY_9");
		PushInteger(L, 0x41, "KEY_A");
		PushInteger(L, 0x42, "KEY_B");
		PushInteger(L, 0x43, "KEY_C");
		PushInteger(L, 0x44, "KEY_D");
		PushInteger(L, 0x45, "KEY_E");
		PushInteger(L, 0x46, "KEY_F");
		PushInteger(L, 0x47, "KEY_G");
		PushInteger(L, 0x48, "KEY_H");
		PushInteger(L, 0x49, "KEY_I");
		PushInteger(L, 0x4A, "KEY_J");
		PushInteger(L, 0x4B, "KEY_K");
		PushInteger(L, 0x4C, "KEY_L");
		PushInteger(L, 0x4D, "KEY_M");
		PushInteger(L, 0x4E, "KEY_N");
		PushInteger(L, 0x4F, "KEY_O");
		PushInteger(L, 0x50, "KEY_P");
		PushInteger(L, 0x51, "KEY_Q");
		PushInteger(L, 0x52, "KEY_R");
		PushInteger(L, 0x53, "KEY_S");
		PushInteger(L, 0x54, "KEY_T");
		PushInteger(L, 0x55, "KEY_U");
		PushInteger(L, 0x56, "KEY_V");
		PushInteger(L, 0x57, "KEY_W");
		PushInteger(L, 0x58, "KEY_X");
		PushInteger(L, 0x59, "KEY_Y");
		PushInteger(L, 0x5A, "KEY_Z");
		PushInteger(L, 0x5B, "KEY_LWIN");
		PushInteger(L, 0x5C, "KEY_RWIN");
		PushInteger(L, 0x5D, "KEY_APPS");
		PushInteger(L, 0x5F, "KEY_SLEEP");
		PushInteger(L, 0x60, "KEY_NUMPAD0");
		PushInteger(L, 0x61, "KEY_NUMPAD1");
		PushInteger(L, 0x62, "KEY_NUMPAD2");
		PushInteger(L, 0x63, "KEY_NUMPAD3");
		PushInteger(L, 0x64, "KEY_NUMPAD4");
		PushInteger(L, 0x65, "KEY_NUMPAD5");
		PushInteger(L, 0x66, "KEY_NUMPAD6");
		PushInteger(L, 0x67, "KEY_NUMPAD7");
		PushInteger(L, 0x68, "KEY_NUMPAD8");
		PushInteger(L, 0x69, "KEY_NUMPAD9");
		PushInteger(L, 0x6A, "KEY_MULTIPLY");
		PushInteger(L, 0x6B, "KEY_ADD");
		PushInteger(L, 0x6C, "KEY_SEPARATOR");
		PushInteger(L, 0x6D, "KEY_SUBTRACT");
		PushInteger(L, 0x6E, "KEY_DECIMAL");
		PushInteger(L, 0x6F, "KEY_DIVIDE");
		PushInteger(L, 0x70, "KEY_F1");
		PushInteger(L, 0x71, "KEY_F2");
		PushInteger(L, 0x72, "KEY_F3");
		PushInteger(L, 0x73, "KEY_F4");
		PushInteger(L, 0x74, "KEY_F5");
		PushInteger(L, 0x75, "KEY_F6");
		PushInteger(L, 0x76, "KEY_F7");
		PushInteger(L, 0x77, "KEY_F8");
		PushInteger(L, 0x78, "KEY_F9");
		PushInteger(L, 0x79, "KEY_F10");
		PushInteger(L, 0x7A, "KEY_F11");
		PushInteger(L, 0x7B, "KEY_F12");
		PushInteger(L, 0x7C, "KEY_F13");
		PushInteger(L, 0x7D, "KEY_F14");
		PushInteger(L, 0x7E, "KEY_F15");
		PushInteger(L, 0x7F, "KEY_F16");
		PushInteger(L, 0x80, "KEY_F17");
		PushInteger(L, 0x81, "KEY_F18");
		PushInteger(L, 0x82, "KEY_F19");
		PushInteger(L, 0x83, "KEY_F20");
		PushInteger(L, 0x84, "KEY_F21");
		PushInteger(L, 0x85, "KEY_F22");
		PushInteger(L, 0x86, "KEY_F23");
		PushInteger(L, 0x87, "KEY_F24");
		PushInteger(L, 0x90, "KEY_NUMLOCK");
		PushInteger(L, 0x91, "KEY_SCROLL");
		PushInteger(L, 0xA0, "KEY_LSHIFT");
		PushInteger(L, 0xA1, "KEY_RSHIFT");
		PushInteger(L, 0xA2, "KEY_LCONTROL");
		PushInteger(L, 0xA3, "KEY_RCONTROL");
		PushInteger(L, 0xA4, "KEY_LMENU");
		PushInteger(L, 0xA5, "KEY_RMENU");
		PushInteger(L, 0xA6, "KEY_BROWSER_BACK");
		PushInteger(L, 0xA7, "KEY_BROWSER_FORWARD");
		PushInteger(L, 0xA8, "KEY_BROWSER_REFRESH");
		PushInteger(L, 0xA9, "KEY_BROWSER_STOP");
		PushInteger(L, 0xAA, "KEY_BROWSER_SEARCH");
		PushInteger(L, 0xAB, "KEY_BROWSER_FAVORITES");
		PushInteger(L, 0xAC, "KEY_BROWSER_HOME");
		PushInteger(L, 0xAD, "KEY_VOLUME_MUTE");
		PushInteger(L, 0xAE, "KEY_VOLUME_DOWN");
		PushInteger(L, 0xAF, "KEY_VOLUME_UP");
		PushInteger(L, 0xB0, "KEY_MEDIA_NEXT_TRACK");
		PushInteger(L, 0xB1, "KEY_MEDIA_PREV_TRACK");
		PushInteger(L, 0xB2, "KEY_MEDIA_STOP");
		PushInteger(L, 0xB3, "KEY_MEDIA_PLAY_PAUSE");
		PushInteger(L, 0xB4, "KEY_LAUNCH_MAIL");
		PushInteger(L, 0xB5, "KEY_LAUNCH_MEDIA_SELECT");
		PushInteger(L, 0xB6, "KEY_LAUNCH_APP1");
		PushInteger(L, 0xB7, "KEY_LAUNCH_APP2");
		PushInteger(L, 0xBA, "KEY_OEM_1");
		PushInteger(L, 0xBB, "KEY_OEM_PLUS");
		PushInteger(L, 0xBC, "KEY_OEM_COMMA");
		PushInteger(L, 0xBD, "KEY_OEM_MINUS");
		PushInteger(L, 0xBE, "KEY_OEM_PERIOD");
		PushInteger(L, 0xBF, "KEY_OEM_2");
		PushInteger(L, 0xDB, "KEY_OEM_4");
		PushInteger(L, 0xDC, "KEY_OEM_5");
		PushInteger(L, 0xDD, "KEY_OEM_6");
		PushInteger(L, 0xDE, "KEY_OEM_7");
		PushInteger(L, 0xDF, "KEY_OEM_8");
		PushInteger(L, 0xE2, "KEY_OEM_102");
		PushInteger(L, 0xE5, "KEY_PROCESSKEY");
		PushInteger(L, 0xE7, "KEY_PACKET");
		PushInteger(L, 0xF6, "KEY_ATTN");
		PushInteger(L, 0xF7, "KEY_CRSEL");
		PushInteger(L, 0xF8, "KEY_EXSEL");
		PushInteger(L, 0xF9, "KEY_EREOF");
		PushInteger(L, 0xFA, "KEY_PLAY");
		PushInteger(L, 0xFB, "KEY_ZOOM");
		PushInteger(L, 0xFC, "KEY_NONAME");
		PushInteger(L, 0xFD, "KEY_PA1");
		PushInteger(L, 0xFE, "KEY_OEM_CLEAR");
	}
}
