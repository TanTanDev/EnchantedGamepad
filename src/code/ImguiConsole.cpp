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
		history.push_back(newData);
	}
	ss.clear();

}

void ImguiConsole::DrawConsole(bool draw)
{
	ImGui::Begin("console");
	ImGui::BeginChild("console log", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	for (unsigned int i = 0; i < history.size(); i++)
	{
		auto histData = history[i];
		ImGui::Text(histData.text.c_str());
	}
	ImGui::SetScrollHere();
	ImGui::EndChild();
	ImGui::End();
}

void ImguiConsole::HandlePrint(const char * string)
{
	historyData newData;
	newData.text = string;
	history.push_back(newData);
}
