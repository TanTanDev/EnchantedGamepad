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
		std::cout << "filepath: " << path << std::endl;
		fileNames.push_back(path);
	}
}

const std::vector<std::string>& Application::GetScriptPaths()
{
	return fileNames;
}
