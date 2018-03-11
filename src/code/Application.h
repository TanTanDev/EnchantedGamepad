#pragma once
// description: this class is currently used to bind together all the logic
#include <vector>

class Application
{
private:
	std::vector<std::string> fileNames;
public:
	void FindScripts(const std::string& directory);
	const std::vector<std::string>& GetScriptPaths();
	Application();
	~Application();
};