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

#include "ProgramsWindow.h"
#include "../imgui/imgui.h"
#include "../Application.h"
#include "../FileScanner.h"
#include "../Script.h"
#include "../ImguiConsole.h"

// todo: is there a smaller lib to use?
// used for finding files in explorer
#include <Windows.h>
#include <fstream>
#include <iostream>

ProgramsWindow::ProgramsWindow()
	:selectedScriptByIndex(-1)
{
}

ProgramsWindow::~ProgramsWindow()
{
}

void ProgramsWindow::Render(Application& FD, FileScanner& fileScanner, Script& script, bool& isRunningScript)
{
	// Menu
	ImGui::Begin("Programs", 0); //ImGuiWindowFlags_MenuBar);// ImGui::SameLine();
	if (ImGui::Button("create new", ImVec2(100.0f, 30.0f)))
	{
		OPENFILENAME ofn;
		char szFileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(OPENFILENAME); // SEE NOTE BELOW
		ofn.hwndOwner = NULL;// GetActiveWindow();
		ofn.lpstrFilter = L"script (*.lua)\0*.lua\0All Files (*.*)\0*.*\0";
		ofn.lpstrInitialDir = L"../Scripts";
		ofn.lpstrFile = (LPWSTR)szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		ofn.lpstrDefExt = L"lua";
		if (GetSaveFileName(&ofn))
		{
			char buffer[260];
			wcstombs(buffer, ofn.lpstrFile, 260);
			HANDLE hFile;
			std::ofstream outFile(buffer);
			outFile << "function Update(dt)\n\nend";
			outFile.close();
			FD.FindScripts("Scripts/");
		}
	}
	ImGui::SameLine();
	auto scripts = FD.GetScriptPaths();

	bool isAnyScriptSelected = (selectedScriptByIndex != -1);
	if (ImGui::Button("delete", ImVec2(50.5f, 30.0f)) && isAnyScriptSelected)
	{
		isRunningScript = false;
		ImGui::OpenPopup("Delete?");
	}
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(sf::Color(140, 140, 140)));
	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(sf::Color(255, 50, 50)));
		ImGui::Text("%s\nWill be deleted.\nThis operation cannot be undone!\n\n", script.GetFileName().c_str());
		ImGui::PopStyleColor();
		ImGui::Separator();
		if (ImGui::Button("OK"))
		{
			script.Unload();
			FD.DeleteFileByIndex(selectedScriptByIndex);
			ImGui::CloseCurrentPopup();
			selectedScriptByIndex = -1;
		}
		ImGui::SameLine();
		if (ImGui::Button("cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor();

	ImGui::Text("Files");
	ImGui::BeginChild("scriptFileList", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

	if (fileScanner.IsPollReady())
	{
		auto changedFiles = fileScanner.PollChanges();

		for (int i = 0; i < changedFiles.size(); i++)
		{
			if (scripts[selectedScriptByIndex].compare("Scripts/" + changedFiles[i]) == 0)
			{
				ImguiConsole::GetInstance().ClearLog(false);
				ImguiConsole::GetInstance().HandlePrint("--- Hot reloading ---", ImguiConsole::LogType::unique);
				script.Unload();
				script.Load(scripts[selectedScriptByIndex].c_str());
				if (isRunningScript && script.HasStartFunction())
					script.InvokeStart();
			}
		}
	}
	for (int i = 0; i < scripts.size(); i++)
	{
		bool changedIndex = false;
		if (i == selectedScriptByIndex)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(sf::Color(100, 230, 90)));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(sf::Color(0, 0, 0, 255)));
		}
		if (ImGui::Button(scripts[i].c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 20)) && i != selectedScriptByIndex)
		{
			selectedScriptByIndex = i;
			changedIndex = true;
			ImguiConsole::GetInstance().ClearLog(false);
			ImguiConsole::GetInstance().HandlePrint("--- start of script ---", ImguiConsole::LogType::unique);
			script.Unload();
			script.Load(scripts[i].c_str());
			isRunningScript = false;
		}
		if (i == selectedScriptByIndex && !changedIndex)
			ImGui::PopStyleColor(2);
	}
	ImGui::EndChild();
	ImGui::End();
}