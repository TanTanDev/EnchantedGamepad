#include "InputBinding.h"
#include "GamepadInput.h"
#include "AppSettings.h"
#include "ImguiConsole.h"

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
				ImguiConsole::GetInstance().HandlePrint(printString);
				
			}
			else
			{
				std::cout << "not a string! DO SOMETHING!?!\n";
			}
		}
		return 1;
	}
	int luaopen_print(lua_State* L)
	{
		lua_getglobal(L, "_G");
		lua_pushcfunction(L, handlePrint);
		lua_setfield(L, -2, "print");
		lua_pop(L, 1);
		return 1;
	}

}
