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
#include "rapidjson/prettywriter.h"

#include <stdio.h>

void ScriptBindingFileManager::CreateBindingsFolder()
{
	//	std::experimental::filesystem::exists(FD.GetWorkingDirectory().string() + "BindingSettings\\");
}


void ScriptBindingFileManager::ApplyBindings(Script& script, std::string bindingFileName)
{
	std::ifstream ifStream(bindingFileName);
	if (ifStream.bad() || ifStream.fail())
	{
		std::cout << "bad/non-existant file: " << bindingFileName << std::endl;
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
		const char* globalName = json["bindingDatas"][i]["bindingName"].GetString();
		const char* bindingDataTypeString = json["bindingDatas"][i]["bindingDataType"].GetString();
		if (strcmp(bindingDataTypeString, "float") == 0)
		{
			float value = (float)json["bindingDatas"][i]["bindingData"].GetDouble();
			script.SetGlobal(globalName, value);
			ScriptBinding::GetInstance().UpdateBinding(globalName, value);
		}
		else if (strcmp(bindingDataTypeString, "int") == 0)
		{
			int value = json["bindingDatas"][i]["bindingData"].GetInt();
			script.SetGlobal(globalName, value);
			ScriptBinding::GetInstance().UpdateBinding(globalName, value);
		}
		else if (strcmp(bindingDataTypeString, "bool") == 0)
		{
			bool value = json["bindingDatas"][i]["bindingData"].GetBool();
			script.SetGlobal(globalName, value);
			ScriptBinding::GetInstance().UpdateBinding(globalName, value);
		}
	}
}


void ScriptBindingFileManager::ConstructUntiteledBinding(std::string scriptFileName)
{
	untiteledBinding.bindingFilesPathRelative.clear();
	untiteledBinding.bindingFilesPathRelative.push_back("UNTITELED");
	untiteledBinding.currentBindingIndex = 0;
	untiteledBinding.scriptFullPath = scriptFileName;
}

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

void ScriptBindingFileManager::CreateNewBindingFileAndSave(std::string filePath)
{
	rapidjson::StringBuffer stringBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
	writer.StartObject();
	writer.Key("bindingDatas");
	writer.StartArray();
	std::vector<Binding> currentBindings = ScriptBinding::GetInstance().GetBindings();
	for (int i = 0; i < currentBindings.size(); i++)
	{
		writer.StartObject();
		writer.Key("bindingName"); writer.String(currentBindings[i].GlobalName.c_str());
		writer.Key("bindingDataType");
		if (currentBindings[i].bindingDataType == Binding::BindingDataType::Bool)
		{
			writer.String("bool");
			writer.Key("bindingData"); writer.Bool(currentBindings[i].bindingData.b);
		}
		if (currentBindings[i].bindingDataType == Binding::BindingDataType::Int)
		{
			writer.String("int");
			writer.Key("bindingData"); writer.Int(currentBindings[i].bindingData.i);
		}
		if (currentBindings[i].bindingDataType == Binding::BindingDataType::Float)
		{
			writer.String("float");
			writer.Key("bindingData"); writer.Double(currentBindings[i].bindingData.f);
		}
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();
	const char* constructedJson = stringBuffer.GetString();

	std::ofstream newFile;
	newFile.open(filePath);
	if (!newFile.is_open())
	{
		std::cout << "OMG cant write new binding file: " << (filePath) << std::endl;
		newFile.close();
		return;
	}
	newFile << constructedJson;
	newFile.close();
}

void ScriptBindingFileManager::AddNewBindingToGlobal(std::string scriptFilePath, std::string bindingFilePath)
{
	auto& bindingItems = this->bindingItemPathsFileData.bindingItems;
	for (int i = 0; i < bindingItems.size(); i++)
	{
		if (bindingItems[i].scriptFullPath.compare(scriptFilePath) == 0)
		{
			bindingItems[i].bindingFilesPathRelative.push_back(bindingFilePath);
			return;
		}
	}
	// construct new path data
	BindingItemPathFileData bindingPathFileData;
	bindingPathFileData.scriptFullPath = scriptFilePath;
	bindingPathFileData.currentBindingIndex = 0;
	bindingPathFileData.bindingFilesPathRelative.push_back(bindingFilePath);
	bindingItems.push_back(bindingPathFileData);
}

void ScriptBindingFileManager::RemoveBindingAndSaveGlobal(std::string scriptFilePath, std::string bindingFilePath)
{
	auto& bindingItems = this->bindingItemPathsFileData.bindingItems;
	for (int i = 0; i < bindingItems.size(); i++)
	{
		if (bindingItems[i].scriptFullPath.compare(scriptFilePath) == 0)
		{
			auto& bindingFilesPaths = bindingItems[i].bindingFilesPathRelative;
			for (int a = 0; a < bindingFilesPaths.size(); a++)
			{
				if (bindingFilesPaths[i].compare(bindingFilePath) == 0)
				{
					bindingFilesPaths.erase(bindingFilesPaths.begin()+i);
					break;
				}
			}
			if (bindingFilesPaths.size() == 0)
			{
				bindingItems.erase(bindingItems.begin() + i);
				break;
			}
		}
	}
	// Delete the file also
	remove(bindingFilePath.c_str());
	SaveGlobalToFile();
}

void ScriptBindingFileManager::SaveGlobalToFile()
{
	rapidjson::StringBuffer stringBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
	writer.StartObject();
	writer.Key("bindingItems");
	writer.StartArray();
	for (int i = 0; i < bindingItemPathsFileData.bindingItems.size(); i++)
	{
		BindingItemPathFileData filePathData = bindingItemPathsFileData.bindingItems[i];
		writer.StartObject();
		writer.Key("scriptFullPath"); writer.String(filePathData.scriptFullPath.c_str());
		writer.Key("currentBindingIndex"); writer.Int(filePathData.currentBindingIndex);
		writer.Key("bindingFiles");
		writer.StartArray();
		for (int a = 0; a < filePathData.bindingFilesPathRelative.size(); a++)
		{
			writer.String(filePathData.bindingFilesPathRelative[a].c_str());
		}
		writer.EndArray();
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();
	const char* constructedJson = stringBuffer.GetString();

	std::ofstream newFile;
	newFile.open("Bindings/_globalBindingSettings");
	if (!newFile.is_open())
	{
		std::cout << "OMG cant write global file!" << std::endl;
		newFile.close();
		return;
	}
	newFile << constructedJson;
	newFile.close();
}

std::vector<std::string> ScriptBindingFileManager::GetBindingFileNames(std::string scriptFilePath)
{
	for (int i = 0; i < bindingItemPathsFileData.bindingItems.size(); i++)
	{
		if (bindingItemPathsFileData.bindingItems[i].scriptFullPath.compare(scriptFilePath) == 0)
		{
			return bindingItemPathsFileData.bindingItems[i].bindingFilesPathRelative;
		}
	}
	ConstructUntiteledBinding(scriptFilePath);
	std::vector <std::string> untiteledArray = std::vector<std::string>();
	untiteledArray.push_back(untiteledBinding.bindingFilesPathRelative[0]);
	return untiteledArray;
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