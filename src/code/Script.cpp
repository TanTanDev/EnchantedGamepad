// This file is part of Enchanted Gamepad
// Copyright (C) <2018> Jonatan Olsson
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Script.h"
#include "ScriptBindings\SimulateInputBinding.h"
#include "ScriptBindings\InputBinding.h"
#include "ScriptBindings\AppSettingsBinding.h"
#include "ScriptBindings\VectorBinding.h"
#include "ScriptBindings\EditorBinding.h"
#include "scriptBindings\PrintBinding.h"
#include "ScriptBindings\rectBinding.h"
#include "windows\BindingsWindow.h"


// used to clear out bindings when unloading
#include "ScriptBinding.h"

// for printing error messages
#include "Windows\ConsoleWindow.h"

#include <iostream>
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

Script::Script()
	: scriptLoaded(false)
	, luaRefUpdate(-1)
	, luaRefStart(-1)
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

	// todo: sandbox
	// for some reason luaopen_package() crashes, which is needed for require.
	luaL_openlibs(luaState);

	ScriptBindings::luaopen_input(luaState);
	ScriptBindings::luaopen_simulateinput(luaState);
	ScriptBindings::luaopen_appsettings(luaState);
	ScriptBindings::luaopen_vector(luaState);
	ScriptBindings::luaopen_editor(luaState);
	ScriptBindings::luaopen_print(luaState);
	ScriptBindings::luaopen_rect(luaState);
	if (luaL_loadfile(luaState, fileName) || lua_pcall(luaState, 0, LUA_MULTRET, 0))
	{
		ConsoleWindow::GetInstance().HandlePrint(lua_tostring(luaState, -1), ConsoleWindow::LogType::error);
		scriptLoaded = false;
		lua_close(luaState);
	}
	else
	{
		scriptLoaded = true;
		lua_getglobal(luaState, "Update");
		luaRefUpdate = luaL_ref(luaState, LUA_REGISTRYINDEX);

		lua_getglobal(luaState, "Start");
		luaRefStart = luaL_ref(luaState, LUA_REGISTRYINDEX);
	}
	BindingsWindow::GetInstance().RefreshBindingFiles(*this);
}

void Script::Unload()
{
	// unregister funcitons
	if (luaRefUpdate)
		luaL_unref(luaState, LUA_REGISTRYINDEX, luaRefUpdate);
	if (luaRefStart)
		luaL_unref(luaState, LUA_REGISTRYINDEX, luaRefStart);
	luaRefUpdate = -1;
	luaRefStart = -1;

	ScriptBinding::GetInstance().ClearBindings();
	fileName.clear();
	if (luaState && scriptLoaded)
		lua_close(luaState);
	scriptLoaded = false;
	BindingsWindow::GetInstance().RefreshBindingFiles(*this);
}

void Script::InvokeUpdate(float dt)
{
	if (!scriptLoaded)
	{
		return;
	}

	if (luaRefUpdate)
	{
		lua_rawgeti(luaState, LUA_REGISTRYINDEX, luaRefUpdate);
		lua_pushnumber(luaState, dt);
		if (0 != lua_pcall(luaState, 1, 0, 0))
		{
			ConsoleWindow::GetInstance().HandlePrint(lua_tostring(luaState, -1), ConsoleWindow::LogType::error);
			lua_pop(luaState, 1);
		}
	}
}

void Script::InvokeStart()
{
	if (luaRefStart)
	{
		lua_rawgeti(luaState, LUA_REGISTRYINDEX, luaRefStart);
		if (0 != lua_pcall(luaState, 0, 0, 0))
		{
			ConsoleWindow::GetInstance().HandlePrint(lua_tostring(luaState, -1), ConsoleWindow::LogType::error);
			lua_pop(luaState, 1);
		}
	}
	BindingsWindow::GetInstance().ApplyBindings(*this);
}

bool Script::IsGood()
{
	return scriptLoaded;
}

const std::string& Script::GetFileName()
{
	return fileName;
}

const bool Script::HasUpdateFunction()
{
	return (luaRefUpdate != -1);
}

const bool Script::HasStartFunction()
{
	return (luaRefStart != -1);
}

lua_State* Script::GetLuaState()
{
	return luaState;
}

void Script::SetGlobal(const char * name, int value)
{
	//lua_getglobal(luaState, name);
	lua_pushnumber(luaState, value);
	lua_setglobal(luaState, name);
}

void Script::SetGlobal(const char * name, float value)
{
	//lua_getglobal(luaState, name);
	lua_pushnumber(luaState, value);
	lua_setglobal(luaState, name);
}

void Script::SetGlobal(const char * name, bool value)
{
	//lua_getglobal(luaState, name);
	
	lua_pushboolean(luaState, value);
	lua_setglobal(luaState, name);
	//lua_pop(luaState, 1);
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
		value = luaL_checkinteger(luaState, -1);
		return true;
	}
	return false;
}
