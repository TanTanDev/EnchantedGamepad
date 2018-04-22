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

#include "ImguiConsole.h"
#include "imgui\imgui.h"
ImguiConsole::ImguiConsole()
	:oldCoutBuffer()
{
	oldCoutBuffer = std::cout.rdbuf(ss.rdbuf());
}

ImguiConsole::~ImguiConsole()
{
	std::cout.rdbuf(oldCoutBuffer);
}

void ImguiConsole::ProccessCout()
{
	std::string to;
	while (std::getline(ss, to, '\n'))
	{
		historyData newData;
		newData.text = to;
		newData.logType = (LogType)(rand()%3);
		history.push_back(newData);
	}
	ss.clear();

}

void ImguiConsole::DrawConsole(bool draw)
{
	ImGui::Begin("console");
	if (ImGui::Button("clear"))
		history.clear();
	ImGui::SameLine();
	ImGui::Checkbox("auto clear", &autoClear);

	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(sf::Color(121)));
	ImGui::BeginChild("console log", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	for (unsigned int i = 0; i < history.size(); i++)
	{
		auto histData = history[i];
		ImVec4 textColor;
		if (histData.logType == LogType::warning)
			textColor = sf::Color(220, 220,20);
		else if (histData.logType == LogType::error)
			textColor = sf::Color(255,30,30);
		else if(histData.logType == LogType::unique)
			textColor = sf::Color(20, 200, 255);
		else
			textColor = sf::Color(220,220,220);
		ImGui::PushStyleColor(ImGuiCol_Text, textColor);

		std::ostringstream stringStream;
		stringStream << i <<": "<< histData.text;
		std::string text = stringStream.str();
		ImGui::TextWrapped(text.c_str());
		ImGui::PopStyleColor();
	}
	ImGui::SetScrollHere();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::End();
}

void ImguiConsole::HandlePrint(const char * string, LogType logType)
{
	historyData newData;
	newData.logType = logType;
	newData.text = string;
	history.push_back(newData);
}


// if force is false, don't clear unless autoClear is set
void ImguiConsole::ClearLog(bool force)
{
	if (force)
		history.clear();
	else if(autoClear)
		history.clear();
}