#include "Application.h"
#include "Script.h"
#include <string>
#include <iostream>
#include <experimental\filesystem>


namespace fs = std::experimental::filesystem;
Application::Application()
{
	currentWorkingDirectory = fs::current_path();
}

Application::~Application()
{
}

std::experimental::filesystem::v1::path Application::GetWorkingDirectory()
{
	return currentWorkingDirectory;
}

void Application::FindScripts(const std::string& directory)
{
	fileNames.clear();
	std::error_code testun;
	fs::v1::path fullPath = currentWorkingDirectory;
	fullPath.append(directory);
	// iterate files in current folder
	for (fs::directory_iterator it(directory, testun); it != fs::directory_iterator(); ++it)
	{
		std::string path = it->path().generic_string();

		int imageNumber;
		bool isLuaFile = (it->path().extension() == ".lua");
		if (!isLuaFile)
		{
			continue;
		}
		fileNames.push_back(path);
	}
	std::cout << testun;
}

void Application::DeleteFileByIndex(int index)
{
	std::experimental::filesystem::remove(fileNames[index]);
	auto itemIter = fileNames.begin() + index;
	fileNames.erase(itemIter, itemIter + 1);
}

const std::vector<std::string>& Application::GetScriptPaths()
{
	return fileNames;
}
