#include "EditorBinding.h"
#include "ScriptBinding.h"
#include <iostream>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
namespace ScriptBindings
{
	const char tableName[] = "Editor";
	inline static void PushFunctions(lua_State* L);

	int luaopen_editor(lua_State* L)
	{
		lua_newtable(L);
		//PushMemberVariables(L);
		PushFunctions(L);
		lua_setglobal(L, tableName);
		return 1;
	}

	static int Bind(lua_State* L)
	{
		lua_Integer key = luaL_checkinteger(L, 1);
		lua_Integer key2 = 0;

		int luaReference = -1;
		BindDataType dataType = BindDataType::number;
		if (lua_isnumber(L, 1))
		{
			dataType = BindDataType::number;
			luaReference = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		if (luaReference == -1)
		{
			std::cout << "[call:Bind] parameter must be a valid data format\n";
		}
		else
		{
			lua_pop(L, 1);
		}
		ScriptBinding::GetInstance().Bind(luaReference, BindDataType::number);

		return 0;
	}

	inline static void PushFunctions(lua_State* L)
	{
		lua_pushcfunction(L, Bind);
		lua_setfield(L, -2, "Bind");
	}

}