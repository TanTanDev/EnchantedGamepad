#pragma once
typedef struct lua_State lua_State;

namespace ScriptBindings
{
	int luaopen_editor(lua_State* L);
}