#pragma once
typedef struct lua_State lua_State;

namespace ScriptBindings
{
	int luaopen_print(lua_State* L);
}
