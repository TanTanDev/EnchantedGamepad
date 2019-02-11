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
#include "Singleton.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

// Singleton. needs to be accessed through lua script
class ConsoleWindow : public Singleton<ConsoleWindow>
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
	ConsoleWindow();
	~ConsoleWindow();
	void ProccessCout();
	void DrawConsole(bool draw);
	void HandlePrint(const char* string, LogType logType = LogType::info);
	void ClearLog(bool force = true);
};