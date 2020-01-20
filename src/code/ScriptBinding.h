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

	union MinMaxData
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

	MinMaxData MinValue;
	MinMaxData MaxValue;
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
	void UpdateBinding(std::string globalName, int value);
	void UpdateBinding(std::string globalName, bool value);
	void UpdateBinding(std::string globalName, float value);
	std::vector<Binding>& GetBindings();
	void ClearBindings();
};