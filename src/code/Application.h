#pragma once
// description: this class is currently used to bind together all the logic
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "JsonHelpers.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <iostream>
#include <vector>
#include <string>


typedef class Script Script;

class ApplicationScriptSettings
{
public:
	ApplicationScriptSettings()
		:directories(0)
	{

	}
	std::vector<std::string> directories;
};

class ApplicationData
{
public:
	ApplicationScriptSettings scriptSettings;
public:
	std::string GetDefaultData()
	{
		rapidjson::StringBuffer s;
		rapidjson::StringBuffer defaultString;
		rapidjson::Writer<rapidjson::StringBuffer> writer(defaultString);
		writer.StartObject();
		writer.Key("scripts");
		writer.StartObject();
		writer.Key("directories");
		writer.StartArray();
		writer.String("this IS A TEST");
		writer.EndArray();
		writer.EndObject();
		writer.EndObject();

		return defaultString.GetString();
	}

	void Serialize(const rapidjson::Document& data)
	{
		if (data.HasMember("scripts"))
		{
			const rapidjson::Value& scriptsData = data["scripts"];
			if (scriptsData.HasMember("directories"))
			{
				const rapidjson::Value& directories = scriptsData["directories"];
				for (rapidjson::SizeType i = 0; i < directories.Size(); i++)
				{
					scriptSettings.directories.push_back(directories[i].GetString());
				}
			}
		}
	}
};


class Application
{
public:
	//Script currentScript;
	ApplicationData applicationData;

	void loadData();
public:
	Application();
	~Application();
};