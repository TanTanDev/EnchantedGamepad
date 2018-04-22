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

#include "VectorBinding.h"
#include "Vector.h"
#include <math.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace ScriptBindings
{
	const char tableName[] = "Vector";

	static int Vector2_x(lua_State* L)
	{
		Vector* Vector = lua_checkvector(L, 1);
		if (lua_isnumber(L, 2))
		{
			Vector->x = static_cast<float>(luaL_checknumber(L, 2));
			return 0;
		}
		else if (lua_isnone(L, 2))
		{
			lua_pushnumber(L, Vector->x);
			return 1;
		}
		return 1;
	}

	static int Vector2_y(lua_State* L)
	{
		Vector* Vector = lua_checkvector(L, 1);
		if (lua_isnumber(L, 2))
		{
			Vector->y = static_cast<float>(luaL_checknumber(L, 2));
			return 0;
		}
		else if (lua_isnone(L, 2))
		{
			lua_pushnumber(L, Vector->y);
			return 1;
		}

		return 1;
	}

	static int Vector2_New(lua_State* L)
	{
		const float x = static_cast<float>(luaL_checknumber(L, 1));
		const float y = static_cast<float>(luaL_checknumber(L, 2));

		lua_pushvector(L, x, y);
		return 1;
	}

	static int Vector2_Zero(lua_State* L)
	{
		lua_pushvector(L, 0.0f, 0.0f);
		return 1;
	}

	static int Vector2___add(lua_State* L)
	{
		Vector& a = *lua_checkvector(L, 1);
		Vector& b = *lua_checkvector(L, 2);

		lua_pushvector(L, a.x + b.x, a.y + b.y);

		return 1;
	}

	static int Vector2___sub(lua_State* L)
	{
		const Vector& a = *lua_checkvector(L, 1);
		const Vector& b = *lua_checkvector(L, 2);

		lua_pushvector(L, a.x - b.x, a.y - b.y);
		return 1;
	}

	static int Vector2___mul(lua_State* L)
	{
		Vector& a = *lua_checkvector(L, 1);

		if (lua_isuserdata(L, 2))
		{
			Vector& b = *lua_checkvector(L, 2);

			lua_pushvector(L, a.x - b.x, a.y - b.y);
		}
		else if (lua_isnumber(L, 2))
		{
			const float b = static_cast<float>(lua_tonumber(L, 2));
			lua_pushvector(L, a.x * b, a.y * b);
		}

		return 1;
	}

	static int Vector2___div(lua_State* L)
	{
		Vector& a = *lua_checkvector(L, 1);
		if (lua_isuserdata(L, 2))
		{
			const Vector& b = *lua_checkvector(L, 2);
			lua_pushvector(L, a.x / b.x, a.y / b.y);
		}
		else if (lua_isnumber(L, 2))
		{
			const float b = static_cast<float>(lua_tonumber(L, 2));
			lua_pushvector(L, a.x / b, a.y / b);
		}
		return 1;
	}

	static int Vector2_Normalize(lua_State* L)
	{
		Vector& LuaVector = *lua_checkvector(L, 1);
		const float Length = sqrt(LuaVector.x * LuaVector.x + LuaVector.y * LuaVector.y);
		if (Length != 0.0f)
		{
			LuaVector.x /= Length;
			LuaVector.y /= Length;
		}
		return 0;
	}

	static int Vector2_Length(lua_State* L)
	{
		const Vector& LuaVector = *lua_checkvector(L, 1);
		lua_pushnumber(L, sqrt(LuaVector.x * LuaVector.x + LuaVector.y * LuaVector.y));
		return 1;
	}

	static int Vector2_Dot(lua_State* L)
	{
		const Vector& LuaVector = *lua_checkvector(L, 1);
		const Vector& ArgVector = *lua_checkvector(L, 2);
		lua_pushnumber(L, LuaVector.x * ArgVector.x + LuaVector.y * ArgVector.y);
		return 1;
	}

	static int Vector2_Set(lua_State* L)
	{
		Vector& LuaVector = *lua_checkvector(L, 1);

		if (lua_isnumber(L, 2) && lua_isnumber(L, 3))
		{
			LuaVector.x = static_cast<float>(luaL_checknumber(L, 2));
			LuaVector.y = static_cast<float>(luaL_checknumber(L, 3));
		}
		else if (lua_isnumber(L, 2))
		{
			LuaVector.x = static_cast<float>(lua_tonumber(L, 2));
			LuaVector.y = static_cast<float>(lua_tonumber(L, 2));
		}

		return 0;
	}

	static int Vector2___tostring(lua_State* L)
	{
		Vector& LuaVector = *lua_checkvector(L, 1);
		char Buffer[256];
		sprintf_s(Buffer, "(%f, %f)", LuaVector.x, LuaVector.y);
		lua_pushstring(L, Buffer);
		return 1;
	}


	static int Vector2___eq(lua_State* L)
	{
		const Vector& a = *lua_checkvector(L, 1);
		const Vector& b = *lua_checkvector(L, 2);
		lua_pushboolean(L, a.x == b.x && a.y == b.y);
		return 1;
	}

	static int Vector2___unm(lua_State* L)
	{
		const Vector& a = *lua_checkvector(L, 1);
		lua_pushvector(L, -a.x, -a.y);
		return 1;
	}

	const luaL_Reg Vector_Functions[] =
	{
		{ "New", Vector2_New },
		{ "Zero", Vector2_Zero },
		{ NULL, NULL }
	};

	inline void Generate_Vector_Methods(lua_State* L)
	{
		lua_pushcfunction(L, Vector2___tostring); lua_setfield(L, -2, "__tostring");
		lua_pushcfunction(L, Vector2___add); lua_setfield(L, -2, "__add");
		lua_pushcfunction(L, Vector2___sub); lua_setfield(L, -2, "__sub");
		lua_pushcfunction(L, Vector2___mul); lua_setfield(L, -2, "__mul");
		lua_pushcfunction(L, Vector2___div); lua_setfield(L, -2, "__div");
		lua_pushcfunction(L, Vector2___eq); lua_setfield(L, -2, "__eq");
		lua_pushcfunction(L, Vector2___unm); lua_setfield(L, -2, "__unm");
		lua_pushvalue(L, -1); lua_setfield(L, -2, "__index");

		lua_pushcfunction(L, Vector2_x); lua_setfield(L, -2, "x");
		lua_pushcfunction(L, Vector2_y); lua_setfield(L, -2, "y");
		lua_pushcfunction(L, Vector2_Normalize); lua_setfield(L, -2, "Normalize");
		lua_pushcfunction(L, Vector2_Length); lua_setfield(L, -2, "Length");
		lua_pushcfunction(L, Vector2_Set); lua_setfield(L, -2, "Set");
		lua_pushcfunction(L, Vector2_Dot); lua_setfield(L, -2, "Dot");
	};

	int luaopen_vector(lua_State* L)
	{
		// create it as metatable
		lua_register(L, tableName, Vector2_New);
		lua_register(L, "Zero", Vector2_Zero);
		luaL_newmetatable(L, tableName);

		Generate_Vector_Methods(L);

		return 1;
	}

	Vector* lua_checkvector(lua_State* L, int n)
	{
		Vector* UserData = static_cast<Vector*>(luaL_checkudata(L, n, tableName));
		char Error[128];
		sprintf_s(Error, "'%s' expected", tableName);
		luaL_argcheck(L, UserData != nullptr, n, Error);
		return UserData;
	}

	Vector lua_checkvectorEx(lua_State* L, int n)
	{
		if (lua_isnumber(L, n))
		{
			lua_Number xPos = luaL_checknumber(L, n);
			lua_Number yPos = luaL_checknumber(L, n+1);
			return Vector(xPos, yPos);
		}
		Vector* vector = lua_checkvector(L, n);
		return *vector;
	}

	int lua_pushvector(lua_State* L, float x, float y)
	{
		void* UserData = lua_newuserdata(L, sizeof(Vector));
		//luaL_setmetatable(L, tableName);
		luaL_getmetatable(L, tableName);
		lua_setmetatable(L, -2);

		Vector* Vec2 = static_cast<Vector*>(UserData);
		Vec2->x = x;
		Vec2->y = y;

		return 1;
	}
}