#pragma once
typedef struct lua_State lua_State;
typedef class Binding Binding;

namespace ScriptBindings
{

	int SetGlobal(lua_State* L, Binding& binding);
	int luaopen_editor(lua_State* L);

	// other bindings  should be able to create vectors
	int lua_pushbinding(lua_State* L, float n);
	//Vector* lua_checkvector(lua_State* L, int n);
	Binding* lua_checkbinding(lua_State* L, int n);

	// checks (x,y) number parameters first
	//Vector lua_checkvectorEx(lua_State* L, int n);
}