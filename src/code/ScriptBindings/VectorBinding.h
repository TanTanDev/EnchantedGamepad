#pragma once
typedef struct lua_State lua_State;
typedef class Vector Vector;

namespace ScriptBindings
{
	int luaopen_vector(lua_State* L);

	// other bindings  should be able to create vectors
	int lua_pushvector(lua_State* L, float x, float y);
	//Vector* lua_checkvector(lua_State* L, int n);
	Vector* lua_checkvector(lua_State* L, int n);

	// checks (x,y) number parameters first
	Vector lua_checkvectorEx(lua_State* L, int n);
}