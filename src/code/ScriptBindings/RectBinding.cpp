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

#include "RectBinding.h"
#include "Rect.h"
#include "VectorBinding.h"
#include <math.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace ScriptBindings
{
	const char tableName[] = "Rect";

	static int Rect_x(lua_State* L)
	{
		Rect* rect = lua_checkrect(L, 1);
		if (lua_isnumber(L, 2))
		{
			rect->X = static_cast<float>(luaL_checknumber(L, 2));
			return 0;
		}
		else if (lua_isnone(L, 2))
		{
			lua_pushnumber(L, rect->X);
			return 1;
		}
		return 1;
	}

	static int Rect_y(lua_State* L)
	{
		Rect* rect = lua_checkrect(L, 1);
		if (lua_isnumber(L, 2))
		{
			rect->Y = static_cast<float>(luaL_checknumber(L, 2));
			return 0;
		}
		else if (lua_isnone(L, 2))
		{
			lua_pushnumber(L, rect->Y);
			return 1;
		}

		return 1;
	}

	static int Rect_width(lua_State* L)
	{
		Rect* rect = lua_checkrect(L, 1);
		if (lua_isnumber(L, 2))
		{
			rect->Width = static_cast<float>(luaL_checknumber(L, 2));
			return 0;
		}
		else if (lua_isnone(L, 2))
		{
			lua_pushnumber(L, rect->Width);
			return 1;
		}
		return 1;
	}

	static int Rect_height(lua_State* L)
	{
		Rect* rect = lua_checkrect(L, 1);
		if (lua_isnumber(L, 2))
		{
			rect->Height = static_cast<float>(luaL_checknumber(L, 2));
			return 0;
		}
		else if (lua_isnone(L, 2))
		{
			lua_pushnumber(L, rect->Height);
			return 1;
		}
		return 1;
	}

	static int Rect_center(lua_State* L)
	{
		Rect* rect = lua_checkrect(L, 1);
		lua_pushvector(L, rect->X + rect->Width*0.5f, rect->Y + rect->Height*0.5f);
		return 1;
	}

	static int Rect_Set(lua_State* L)
	{
		Rect& luaRect = *lua_checkrect(L, 1);

		if (lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4), lua_isnumber(L, 5))
		{
			luaRect.X = static_cast<float>(luaL_checknumber(L, 2));
			luaRect.Y = static_cast<float>(luaL_checknumber(L, 3));
			luaRect.Width = static_cast<float>(luaL_checknumber(L, 4));
			luaRect.Height = static_cast<float>(luaL_checknumber(L, 5));
		}
		return 0;
	}

	static int Rect_New(lua_State* L)
	{
		const float x = static_cast<float>(luaL_checknumber(L, 1));
		const float y = static_cast<float>(luaL_checknumber(L, 2));
		const float w = static_cast<float>(luaL_checknumber(L, 3));
		const float h = static_cast<float>(luaL_checknumber(L, 4));

		lua_pushrect(L, x, y, w, h);
		return 1;
	}

	static int Rect___tostring(lua_State* L)
	{
		Rect& luaRect = *lua_checkrect(L, 1);
		char Buffer[256];
		sprintf_s(Buffer, "(x: %f, y: %f, w:%f, h: %f)", luaRect.X, luaRect.Y, luaRect.Width, luaRect.Height);
		lua_pushstring(L, Buffer);
		return 1;
	}

	const luaL_Reg Vector_Functions[] =
	{
		{ "New", Rect_New },
		{ NULL, NULL }
	};

	inline void Generate_Rect_Methods(lua_State* L)
	{
		lua_pushcfunction(L, Rect___tostring); lua_setfield(L, -2, "__tostring");
		lua_pushvalue(L, -1); lua_setfield(L, -2, "__index");

		lua_pushcfunction(L, Rect_x); lua_setfield(L, -2, "X");
		lua_pushcfunction(L, Rect_y); lua_setfield(L, -2, "Y");
		lua_pushcfunction(L, Rect_width); lua_setfield(L, -2, "Width");
		lua_pushcfunction(L, Rect_height); lua_setfield(L, -2, "Height");
		lua_pushcfunction(L, Rect_Set); lua_setfield(L, -2, "Set");
		lua_pushcfunction(L, Rect_center); lua_setfield(L, -2, "Center");
	};

	int luaopen_rect(lua_State* L)
	{
		// create it as metatable
		lua_register(L, tableName, Rect_New);
		luaL_newmetatable(L, tableName);

		Generate_Rect_Methods(L);

		return 1;
	}

	Rect* lua_checkrect(lua_State* L, int n)
	{
		Rect* UserData = static_cast<Rect*>(luaL_checkudata(L, n, tableName));
		char Error[128];
		sprintf_s(Error, "'%s' expected", tableName);
		luaL_argcheck(L, UserData != nullptr, n, Error);
		return UserData;
	}

	int lua_pushrect(lua_State* L, float x, float y, float width, float height)
	{
		void* UserData = lua_newuserdata(L, sizeof(Rect));
		//luaL_setmetatable(L, tableName);
		luaL_getmetatable(L, tableName);
		lua_setmetatable(L, -2);

		Rect* rect = static_cast<Rect*>(UserData);
		rect->X = x;
		rect->Y = y;
		rect->Width = width;
		rect->Height = height;
		return 1;
	}
}