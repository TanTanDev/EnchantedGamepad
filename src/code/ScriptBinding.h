#pragma once
#include "Singleton.h"

#define MAXBINDINGS 10

enum BindDataType
{
	number
};


union BindingDataCopy
{
	int numberValue;
};

struct Binding
{
	BindDataType bindType;
	int luaReference;

	// C++ side data copy of Lua object
	BindingDataCopy dataBindingCopy;
};

class ScriptBinding : public Singleton<ScriptBinding>
{
private:
	Binding bindings[MAXBINDINGS];
	int bindingsCount = 0;
public:
	ScriptBinding();
	~ScriptBinding();

	void Bind(int ref, BindDataType dataType);
	Binding* GetBindings();
};