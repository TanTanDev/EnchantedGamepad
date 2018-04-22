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

#include "ScriptBinding.h"
#include <iostream>

//void ScriptBinding::Bind(int ref, BindDataType dataType)
//{
	//bindingsCount++;
	//if (bindingsCount > MAXBINDINGS)
	//{
	//	std::cout << "TO MANY SCRIPT BINDINGS!\n";
	//	return;
	//}
	//Binding binding = bindings[bindingsCount];
	//binding.bindType = dataType;
	//binding.luaReference = ref;
	//std::cout << "BINDED: " << ref << std::endl;
//}

ScriptBinding::ScriptBinding()
	:bindings()
{
}

ScriptBinding::~ScriptBinding()
{
}

void ScriptBinding::Bind(Binding binding)
{
	bindings.push_back(binding);
	//Binding binding = bindings[bindingsCount];
	//binding.bindType = dataType;
	//binding.luaReference = ref;
	//std::cout << "BINDED: " << ref << std::endl;
}

std::vector<Binding>& ScriptBinding::GetBindings()
{
	return bindings;
}

void ScriptBinding::ClearBinding()
{
	bindings.clear();
}
