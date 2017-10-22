#pragma once
typedef struct lua_State lua_State;

namespace ScriptBindings
{
	int luaopen_input(lua_State* L);
}
