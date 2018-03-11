#pragma once
#include "Singleton.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
class ImguiConsole: public Singleton<ImguiConsole>
{
private:
	struct historyData
	{
		sf::Color color;
		std::string text;
	};
	std::vector<historyData> history;
	std::stringstream ss;
	std::streambuf* oldCoutBuffer;
public:
	ImguiConsole();
	~ImguiConsole();
	void ProccessCout();
	void DrawConsole(bool draw);
	void HandlePrint(const char* string);
};