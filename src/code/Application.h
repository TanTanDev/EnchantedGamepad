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