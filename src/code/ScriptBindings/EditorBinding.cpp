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

//#include "VectorBinding.h"
//#include "Vector.h"
#include "EditorBinding.h"
#include "ScriptBinding.h"
#include "AppSettings.h"
#include "RectBinding.h"
#include "Rect.h"
//#include <math.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace ScriptBindings
{
	const char tableName[] = "Bind";

	int SetGlobal(lua_State* L, Binding& binding)
	{
		lua_getglobal(L, binding.GlobalName.c_str());
		if (binding.bindingDataType == Binding::BindingDataType::Float)
			lua_pushnumber(L, binding.bindingData.f);
		else if (binding.bindingDataType == Binding::BindingDataType::Bool)
			lua_pushboolean(L, binding.bindingData.b);
		lua_setglobal(L, binding.GlobalName.c_str());
		return 1;
	}

	static int Binding_New(lua_State* L)
	{
		Binding binding;
		std::string globalName = luaL_checkstring(L, 1);
		binding.GlobalName = globalName;
		bool isInt;
		// check for edit type first so we know if we are reading an int or float
		if (lua_isstring(L, 3))
		{
			// edit type
			const char* editType = luaL_checkstring(L, 3);
			if (strcmp(editType, "InputFloat") == 0)
				binding.editType = Binding::EditType::InputFloat;
			else if (strcmp(editType, "DragFloat") == 0)
				binding.editType = Binding::EditType::DragFloat;
			else if (strcmp(editType, "SliderFloat") == 0)
				binding.editType = Binding::EditType::SliderFloat;
			else if (strcmp(editType, "InputInt") == 0)
				binding.editType = Binding::EditType::InputInt;
			else if (strcmp(editType, "DragInt") == 0)
				binding.editType = Binding::EditType::DragInt;
			else if (strcmp(editType, "SliderInt") == 0)
				binding.editType = Binding::EditType::SliderInt;
			else if (strcmp(editType, "ToggleBox") == 0)
				binding.editType = Binding::EditType::ToggleBox;
			else if (strcmp(editType, "ToggleButton") == 0)
				binding.editType = Binding::EditType::ToggleButton;
			isInt = !(strcmp(editType, "InputFloat") == 0 || strcmp(editType, "DragFloat") == 0 || strcmp(editType, "SliderFloat") == 0);

			// set default min and max value
			(isInt) ? binding.MinValue.i = 0: binding.MinValue.f = 0.0f;
			(isInt) ? binding.MaxValue.i = 1 : binding.MaxValue.f = 1.0f;

			// min max
			if (lua_isnumber(L, 4))
			{
				float minValue = luaL_checknumber(L, 4);
				(isInt) ? binding.MinValue.i = (int)minValue : binding.MinValue.f = minValue;
			}
			if (lua_isnumber(L, 5))
			{
				float maxValue = luaL_checknumber(L, 5);
				(isInt) ? binding.MaxValue.i = (int)maxValue : binding.MaxValue.f = maxValue;
			}
		}

		if (lua_isnumber(L, 2))
		{
			lua_Number startValue = luaL_checknumber(L, 2);
			(isInt) ? binding.Set((int)startValue) : binding.Set((float)startValue);
		}
		else if (lua_isboolean(L, 2))
		{
			bool startValue = lua_toboolean(L, 2);
			binding.Set(startValue);
		}

		if (lua_isnumber(L, 3))
		{
			luaL_checknumber(L, 3);
		}

		ScriptBinding::GetInstance().Bind(binding);

		// Bind funciton returns the data specified in the parameter
		if(binding.bindingDataType == Binding::BindingDataType::Float)
			lua_pushnumber(L, binding.bindingData.f);
		else if (binding.bindingDataType == Binding::BindingDataType::Bool)
			lua_pushboolean(L, binding.bindingData.b);
		if (binding.bindingDataType == Binding::BindingDataType::Int)
			lua_pushinteger(L, binding.bindingData.i);
		return 1;
	}

	static int GetWindowRect(lua_State* L)
	{
		const char* windowName = luaL_checkstring(L, 1);
		Rect rectData = AppSettings::GetInstance().GetWindowRectByName(windowName);
		lua_pushrect(L, rectData.X, rectData.Y, rectData.Width, rectData.Height);
		return 1;
	}

	const luaL_Reg Binding_Functions[] =
	{
		{ "Bind", Binding_New },
		{ NULL, NULL }
	};

	inline void Generate_Binding_Methods(lua_State* L)
	{
		lua_pushvalue(L, -1); lua_setfield(L, -2, "__index");
	};

	int luaopen_editor(lua_State* L)
	{
		lua_register(L, "Bind", Binding_New);
		lua_register(L, "GetWindowRect", GetWindowRect);
		return 1;
	}

	Binding* lua_checkbinding(lua_State* L, int n)
	{
		Binding* UserData = static_cast<Binding*>(luaL_checkudata(L, n, tableName));
		char Error[128];
		sprintf_s(Error, "'%s' expected", tableName);
		luaL_argcheck(L, UserData != nullptr, n, Error);
		return UserData;
	}
}