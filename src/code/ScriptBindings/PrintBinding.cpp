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
#include "Windows\ConsoleWindow.h"

#include <iostream>
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace ScriptBindings
{
	static int handlePrint(lua_State* L)
	{
		int nargs = lua_gettop(L);
		for (int i = 1; i <= nargs; i++)
		{
			if (lua_isstring(L, i))
			{
				const char* printString = lua_tostring(L, i);
				ConsoleWindow::GetInstance().HandlePrint(printString);
				
			}
			else
			{
				std::cout << "not a string! DO SOMETHING!?!\n";
			}
		}
		return 0;
	}

	static int clearLog(lua_State* L)
	{
		ConsoleWindow::GetInstance().ClearLog(true);
		return 0;
	}

	int luaopen_print(lua_State* L)
	{
		lua_getglobal(L, "_G");
		lua_pushcfunction(L, handlePrint);
		lua_setfield(L, -2, "print");
		lua_pushcfunction(L, clearLog);
		lua_setfield(L, -2, "ClearLog");
		lua_pop(L, 1);
		return 1;
	}

}
