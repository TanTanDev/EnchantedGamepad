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
// description: this class is currently used to bind together all the logic
#include <vector>
#include <filesystem>

class Application
{
private:
	std::vector<std::string> fileNames;
	std::experimental::filesystem::v1::path currentWorkingDirectory;

public:
	std::experimental::filesystem::v1::path GetWorkingDirectory();
	void FindScripts(const std::string& directory);
	void DeleteFileByIndex(int index);
	const std::vector<std::string>& GetScriptPaths();

	Application();
	~Application();
};