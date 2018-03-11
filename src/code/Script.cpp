#include "Script.h"
#include "ScriptBindings\SimulateInputBinding.h"
#include "ScriptBindings\InputBinding.h"
#include "ScriptBindings\AppSettingsBinding.h"
#include "ScriptBindings\VectorBinding.h"
#include "ScriptBindings\EditorBinding.h"
#include "scriptBindings\PrintBinding.h"

#include <iostream>
extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

Script::Script()
	:scriptLoaded(false)
{
}

Script::~Script()
{
	Unload();
}

void Script::Load(const char* fileName)
{
	//fileName.assign(fileName);
	this->fileName = std::string(fileName);
	//fileName = std::string(fileName);
	luaState = luaL_newstate();

	luaopen_base(luaState);
	luaopen_math(luaState);
	luaopen_string(luaState);
	luaopen_table(luaState);
	
	// todo: sandbox
	luaL_openlibs(luaState);

	ScriptBindings::luaopen_input(luaState);
	ScriptBindings::luaopen_simulateinput(luaState);
	ScriptBindings::luaopen_appsettings(luaState);
	ScriptBindings::luaopen_vector(luaState);
	ScriptBindings::luaopen_editor(luaState);
	ScriptBindings::luaopen_print(luaState);
	if (luaL_loadfile(luaState, fileName) || lua_pcall(luaState, 0, LUA_MULTRET, 0))
	{
		printf("Cannot load script: %s\n", fileName);
		printf(lua_tostring(luaState, -1));
		scriptLoaded = false;
		lua_close(luaState);
	}
	else
	{
		scriptLoaded = true;
		lua_getglobal(luaState, "Update");
		if (lua_isfunction(luaState, -1))
			luaRefUpdate = luaL_ref(luaState, LUA_REGISTRYINDEX);
		//else
			//printf("Script does not have the Function Update()");
		lua_pop(luaState, 1);
	}
}

void Script::Unload()
{
	fileName.empty();
	if (luaState && scriptLoaded)
		lua_close(luaState);
}

void Script::Run(float dt)
{
	if(!scriptLoaded)
	{
		return;
	}

	if (luaRefUpdate)
	{
		lua_rawgeti(luaState, LUA_REGISTRYINDEX, luaRefUpdate);
		lua_pushnumber(luaState, dt);
		if (0 != lua_pcall(luaState, 1, 0, 0))
		{
			printf("%s\n", lua_tostring(luaState, -1));
			lua_pop(luaState, 1);
		}
	}
}

const std::string& Script::GetFileName()
{
	return fileName;
}

bool Script::Get(const char * name, int& value)
{
	if (!scriptLoaded)
	{
		return false;
	}
	lua_getglobal(luaState, name);
	if (lua_isnil(luaState, -1) || !lua_isnumber(luaState, -1))
	{
		// Right now error handling is implemented by caller
		return false;
	}
	else
	{
		value = luaL_checkinteger(luaState,-1);
		return true;
	}
	return false;
}
