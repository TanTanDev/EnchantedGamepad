#pragma once
#include "Singleton.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
class ImguiConsole: public Singleton<ImguiConsole>
{
public:
	enum LogType
	{
		info,
		warning,
		error,
		unique
	};
private:
	struct historyData
	{
		LogType logType;
		std::string text;
	};
	std::vector<historyData> history;
	std::stringstream ss;
	std::streambuf* oldCoutBuffer;

	bool autoClear;
public:
	ImguiConsole();
	~ImguiConsole();
	void ProccessCout();
	void DrawConsole(bool draw);
	void HandlePrint(const char* string, LogType logType = LogType::info);
	void ClearLog(bool force = true);
};