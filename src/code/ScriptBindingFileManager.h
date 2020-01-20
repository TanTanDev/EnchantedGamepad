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
#include <string>
#include <vector>
typedef class Script Script;


class BindingFileData
{
public:
	enum BindingDataType
	{
		Float,
		Bool,
		Int
	};

	// the actual data 
	union Data
	{
		float f;
		bool b;
		int i;
	};

	Data bindingData;
	BindingDataType bindingDataType;

	std::string GlobalName;
	BindingFileData()
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

struct BindingDatasFileData
{
	std::vector<BindingFileData> bindingDatas;
};


struct BindingItemPathFileData
{
	std::string scriptFullPath;
	std::vector<std::string> bindingFilesPathRelative;
	int currentBindingIndex;
};

struct BindingItemPathsFileData
{
	std::vector<BindingItemPathFileData> bindingItems;
};


class ScriptBindingFileManager : public Singleton<ScriptBindingFileManager>
{
private:
	void CreateBindingsFolder();

	BindingItemPathsFileData bindingItemPathsFileData;
	BindingItemPathFileData untiteledBinding;

	void ConstructUntiteledBinding(std::string scriptFileName);
public:
	ScriptBindingFileManager();
	~ScriptBindingFileManager();
	void CreateOrLoadGlobalBindingsFile();
	void CreateNewBindingFileAndSave(std::string filePath);
	void AddNewBindingToGlobal(std::string scriptFilePath, std::string bindingFilePath);
	void RemoveBindingAndSaveGlobal(std::string scriptFilePath, std::string bindingFilePath);
	void SaveGlobalToFile();

	void ApplyBindings(Script& script, std::string bindingFileName);
	std::vector<std::string>GetBindingFileNames(std::string scriptFilePath);
	/*void ApplyBindings(Script& script);*/
	/*void ApplyBindings(Script& script, int index);*/
};