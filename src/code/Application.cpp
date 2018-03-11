#include "Application.h"
#include "Script.h"
#include <string>
#include <iostream>
#include <experimental\filesystem>


namespace fs = std::experimental::filesystem;
Application::Application()
{
}

Application::~Application()
{
}

void Application::FindScripts(const std::string& directory)
{
	// iterate files in current folder
	for (fs::directory_iterator it(directory); it != fs::directory_iterator(); ++it)
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
