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
#include "AppSettings.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace ScriptBindings
{
	const char tableName[] = "AppSettings";
	inline static void PushMemberVariables(lua_State* L);
	inline static void PushInteger(lua_State* L, lua_Number v, const char* n);

	int luaopen_appsettings(lua_State* L)
	{
		lua_newtable(L);
		PushMemberVariables(L);
		lua_setglobal(L, tableName);
		return 1;
	}

	inline static void PushInteger(lua_State* L, lua_Number v, const char* n)
	{
		lua_pushnumber(L, v);
		lua_setfield(L, -2, n);
	}

	inline static void PushMemberVariables(lua_State* L)
	{
		PushInteger(L, AppSettings::GetInstance().screenResolutionWidth, "ScreenResolutionX");
		PushInteger(L, AppSettings::GetInstance().screenResolutionHeight, "ScreenResolutionY");
	}
}
