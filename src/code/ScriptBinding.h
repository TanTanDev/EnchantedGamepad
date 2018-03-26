#pragma once
#include "Singleton.h"
#include <iostream>
#include <vector>

enum BindDataType
{
	number
};

class Binding
{
public:

	enum BindingDataType
	{
		Float,
		Bool,
		Int
	};

	union BindingData
	{
		float f;
		bool b;
		int i;
	};

	union MinMaxDataType
	{
		float f;
		int i;
	};

	enum EditType
	{
		InputFloat,
		DragFloat,
		SliderFloat,
		InputInt,
		DragInt,
		SliderInt,
		ToggleBox,
		ToggleButton
	};

	BindingData bindingData;
	BindingDataType bindingDataType;
	EditType editType;

	MinMaxDataType MinValue;
	MinMaxDataType MaxValue;
	std::string GlobalName;
	Binding()
	{
	}

	void Set(float f)
	{
		bindingData.f = f;
		bindingDataType = Float;
	}
	void Set(bool b)
	{
		bindingData.b = b;
		bindingDataType = Bool;
	}
	void Set(int i)
	{
		bindingData.i = i;
		bindingDataType = Int;
	}
};


class ScriptBinding : public Singleton<ScriptBinding>
{
private:
	std::vector<Binding> bindings;
public:
	ScriptBinding();
	~ScriptBinding();

	void Bind(Binding binding);
	std::vector<Binding>& GetBindings();
	void ClearBinding();
};