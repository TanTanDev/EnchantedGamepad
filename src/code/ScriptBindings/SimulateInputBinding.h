#pragma once
typedef struct lua_State lua_State;

namespace ScriptBindings
{
	int luaopen_simulatedinput(lua_State* L);
}