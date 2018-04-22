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
typedef struct lua_State lua_State;
typedef class Rect Rect;

namespace ScriptBindings
{
	int luaopen_rect(lua_State* L);

	// other bindings  should be able to create vectors
	int lua_pushrect(lua_State* L, float x, float y, float w, float h);
	//Vector* lua_checkvector(lua_State* L, int n);
	Rect* lua_checkrect(lua_State* L, int n);

}