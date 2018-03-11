#include "ScriptBinding.h"
#include <iostream>

void ScriptBinding::Bind(int ref, BindDataType dataType)
{
	bindingsCount++;
	if (bindingsCount > MAXBINDINGS)
	{
		std::cout << "TO MANY SCRIPT BINDINGS!\n";
		return;
	}
	Binding binding = bindings[bindingsCount];
	binding.bindType = dataType;
	binding.luaReference = ref;
}

ScriptBinding::ScriptBinding()
{
}

ScriptBinding::~ScriptBinding()
{
}
