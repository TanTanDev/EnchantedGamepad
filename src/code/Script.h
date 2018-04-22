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

#pragma once
#include <string>
typedef struct lua_State lua_State;
class Script
{
public:
	Script();
	~Script();
	void Load(const char* fileName);
	void Unload();
	void Run(float dt);
	bool Get(const char* name, int& value);

	const std::string& GetFileName();
	const bool HasUpdateFunction();
	lua_State* GetLuaState();
private:
	std::string fileName;
	lua_State* luaState;
	int luaRefUpdate;
	bool scriptLoaded;
};