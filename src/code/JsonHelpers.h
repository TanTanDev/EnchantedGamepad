#pragma once
#include "rapidjson/document.h"
#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
namespace JsonHelpers
{
	inline rapidjson::Document CreateOrOpen(const std::wstring& fileName,  std::string defaultData)
	{
		rapidjson::Document outDocument;
		std::ifstream Stream(fileName);
		if (!Stream.is_open())
		{
			std::wcerr << L"Could not open json file, creating default: " << fileName << std::endl;

			// create file
			std::ofstream newFile;
			newFile.open(fileName);
			newFile << defaultData;
			newFile.close();
			outDocument.Parse<0ui32>(defaultData.c_str());
		}
		else
		{
			std::string Json((std::istreambuf_iterator<char>(Stream)), std::istreambuf_iterator<char>());
			outDocument.Parse< 0ui32 >(Json.c_str());
		}

		if (outDocument.HasParseError())
		{
			std::wcerr << L"json parsing error in file: " << fileName << std::endl;
		}
		return outDocument;
	}
}