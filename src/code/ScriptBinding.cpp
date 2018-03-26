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
