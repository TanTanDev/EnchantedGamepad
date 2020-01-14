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
			newFile.open(fileName, std::fstream::out);
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