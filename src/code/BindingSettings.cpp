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

#include "BindingSettings.h"
#include "Script.h"

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
void BindingSettings::CreateBindingsFolder()
{
//	std::experimental::filesystem::exists(FD.GetWorkingDirectory().string() + "BindingSettings\\");
}

BindingSettings::BindingSettings()
{

}

BindingSettings::~BindingSettings()
{
}

void BindingSettings::CreateDefaultFile(std::string scriptRelativePath)
{
	std::string defaultStr = "{\n    \"fullPath\": \"" + scriptRelativePath + "\",\n    \"defaultSettingsIndex\": 0,\n    \"bindingTemplate\":\n    [\n    ],\n    \"settings\":\n    [\n    ]\n}";
	std::ofstream outFile(scriptRelativePath);
	if (!outFile.good())
	{
		
	}
	outFile.write(defaultStr.c_str(),defaultStr.size());
	outFile.close();
}