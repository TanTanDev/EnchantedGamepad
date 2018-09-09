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

#include "Application.h"
#include "Script.h"
#include "Rect.h"
#include <string>
#include <iostream>
#include <experimental\filesystem>
#include <Windows.h>


namespace fs = std::experimental::filesystem;
Application::Application()
{
	currentWorkingDirectory = fs::current_path();
	rootFolder.folderName = "Scripts";
}

Application::~Application()
{
}

std::experimental::filesystem::v1::path Application::GetWorkingDirectory()
{
	return currentWorkingDirectory;
}

void Application::FindFilesRecursive(std::string directory, FolderData& currentFolder)
{
	std::error_code errorCode;
	// iterate files in current folder
	for (fs::directory_iterator it(directory, errorCode); it != fs::directory_iterator(); ++it)
	{
		std::string path = it->path().generic_string();
		auto fileName = it->path().filename().string();
		bool isLuaFile = (it->path().extension() == ".lua");
		bool isDirectory = !(it->path().has_extension());

		if (isLuaFile)
		{
			currentFolder.filesNames.push_back(fileName);
			currentFolder.fullPathfilesNames.push_back(path);
			
		}
		else if (isDirectory)
		{
			FolderData newFolderData;
			newFolderData.childCount = 0;
			newFolderData.folderName = fileName;
			currentFolder.subfolders.push_back(newFolderData);
			FindFilesRecursive(path, currentFolder.subfolders.back());
		}
	}
}

void Application::FindScripts(const std::string& directory)
{
	//fileNames.clear();
	fs::v1::path fullPath = currentWorkingDirectory;
	fullPath.append(directory);
	FindFilesRecursive(directory, rootFolder);
}

void Application::DeleteFileByIndex(int index)
{
	//std::experimental::filesystem::remove(fileNames[index]);
	//auto itemIter = fileNames.begin() + index;
	//fileNames.erase(itemIter, itemIter + 1);
}

const std::vector<std::string>& Application::GetScriptPaths()
{
	return rootFolder.filesNames;
	// TODO: insert return statement here
}

const FolderData & Application::GetRootFolder()
{
	return rootFolder;
}
