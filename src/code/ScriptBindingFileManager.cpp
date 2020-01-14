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

#include "ScriptBindingFileManager.h"
#include "ScriptBindings\EditorBinding.h"
#include "ScriptBinding.h"
#include "Script.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "JsonHelpers.h"
void ScriptBindingFileManager::CreateBindingsFolder()
{
//	std::experimental::filesystem::exists(FD.GetWorkingDirectory().string() + "BindingSettings\\");
}

/*
void ScriptBindingFileManager::ApplyBindingFromFileName(Script& script, std::string fileName)
{
	// Load file and generate a binding settings
	//bindingItemPathsFileData.bindingDatas.clear();
	std::ifstream ifStream(fileName);
	if (ifStream.bad())
	{
		std::cout << "bad/non-existant file: " << fileName << std::endl;
		return;
	}
	std::string contents = std::string(std::istreambuf_iterator<char>(ifStream), std::istreambuf_iterator<char>());
	//rapidjson::Document(contents);
	rapidjson::Document json;
	json.Parse(contents.c_str());

	if (!json.HasMember("bindingDatas") || !json["bindingDatas"].IsArray())
	{
		return;
	}
	for (int i = 0; i < json["bindingDatas"].Size(); i++)
	{
		if (!json["bindingDatas"][i].IsObject())
		{
			return;
		}
		BindingData bindingData = BindingData();
		std::string globalName = json["bindingDatas"][i]["bindingName"].GetString();
		const char* bindingDataTypeString = json["bindingDatas"][i]["bindingDataType"].GetString();
		if (strcmp(bindingDataTypeString, "float") == 0)
			ScriptBinding::GetInstance().SetBinding(globalName, (float)json["bindingDatas"][i]["bindingData"].GetDouble());
		else if(strcmp(bindingDataTypeString, "int") == 0)
			ScriptBinding::GetInstance().SetBinding(globalName, json["bindingDatas"][i]["bindingData"].GetInt());
		else if (strcmp(bindingDataTypeString, "bool") == 0)
			ScriptBinding::GetInstance().SetBinding(globalName, json["bindingDatas"][i]["bindingData"].GetBool());
		CurrentBindingDatas.bindingDatas.push_back(bindingData);
	}
	for (int i = 0; i < CurrentBindingDatas.bindingDatas.size(); i++)
	{
		BindingData bindingData = CurrentBindingDatas.bindingDatas[i];
		if(bindingData.bindingDataType == BindingData::BindingDataType::Bool)
			script.SetGlobal(bindingData.GlobalName.c_str(), bindingData.bindingData.b);
		else if (bindingData.bindingDataType == BindingData::BindingDataType::Int)
			script.SetGlobal(bindingData.GlobalName.c_str(), bindingData.bindingData.i);
		else if (bindingData.bindingDataType == BindingData::BindingDataType::Float)
			script.SetGlobal(bindingData.GlobalName.c_str(), bindingData.bindingData.f);
	}
}
*/

ScriptBindingFileManager::ScriptBindingFileManager()
{

}

ScriptBindingFileManager::~ScriptBindingFileManager()
{
}

void ScriptBindingFileManager::CreateOrLoadGlobalBindingsFile()
{
	const char* defaultFile =
		"{\
		\"bindingItems\": [\
		]\
		}\
		";

	rapidjson::Document jsonDoc = JsonHelpers::CreateOrOpen(L"Bindings/_globalBindingSettings", defaultFile);
	if (!jsonDoc.HasMember("bindingItems") || !jsonDoc["bindingItems"].IsArray())
		return;

	for (int i = 0; i < jsonDoc["bindingItems"].Size(); i++)
	{
		if (!jsonDoc["bindingItems"][i].IsObject())
			continue;
		if (!jsonDoc["bindingItems"][i].HasMember("scriptFullPath"))
			continue;
		const char* fullPath = jsonDoc["bindingItems"][i]["scriptFullPath"].GetString();
		BindingItemPathFileData bindingItem = BindingItemPathFileData();
		bindingItem.scriptFullPath = std::string(fullPath);

		if (jsonDoc["bindingItems"][i].HasMember("bindingFiles"))
		{
			if (!jsonDoc["bindingItems"][i]["bindingFiles"].IsArray())
				continue;
			for (int a = 0; a < jsonDoc["bindingItems"][i]["bindingFiles"].Size(); a++)
			{
				const char* bindingItemName = jsonDoc["bindingItems"][i]["bindingFiles"][a].GetString();
				bindingItem.bindingFilesPathRelative.push_back(std::string(bindingItemName));
			}
		}
		bindingItemPathsFileData.bindingItems.push_back(bindingItem);
	}
}
/*
void ScriptBindingFileManager::ApplyBindings(Script& script)
{
	std::string scriptFileName = script.GetFileName();
	std::vector<BindingItemPath> bindingItems = GlobalBindingSettings.bindingItems;
	for (int i = 0; i < bindingItems.size(); i++)
	{
		if (bindingItems[i].scriptFullPath.compare(scriptFileName) == 0)
		{
			int index = bindingItems[i].currentBindingIndex;
			if (index < bindingItems[i].bindingFilesPathRelative.size())
			{
				std::string bindingFileName = bindingItems[i].bindingFilesPathRelative[index];
				ApplyBindingFromFileName(script, bindingFileName);
			}
			return;
		}
	}
}
*/