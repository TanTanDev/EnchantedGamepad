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
