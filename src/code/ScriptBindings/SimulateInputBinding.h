#pragma once
typedef struct lua_State lua_State;

namespace ScriptBindings
{
	int luaopen_simulateinput(lua_State* L);
}