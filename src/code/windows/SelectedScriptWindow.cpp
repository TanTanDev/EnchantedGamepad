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
#include "SelectedScriptWindow.h"

#include "../SimulateInput.h"

#include "../imgui/imgui.h"
#include "../Application.h"
#include "../FileScanner.h"
#include "../Script.h"
#include "../Windows/ConsoleWindow.h"
#include "../ScriptBinding.h"

SelectedScriptWindow::SelectedScriptWindow()
{
}

SelectedScriptWindow::~SelectedScriptWindow()
{
}

void SelectedScriptWindow::Render(Application& FD, FileScanner& fileScanner, Script& script, bool& isRunningScript, bool& autoHotReload)
{
	ImGui::Begin("Selected Script", 0);
	ImGui::Text(script.GetFileName().c_str());
	if (ImGui::Checkbox("hot reload", &autoHotReload))
	{
		auto fullPath = FD.GetWorkingDirectory().append("Scripts");
		if (autoHotReload)
			fileScanner.BeginWatchDirectory(fullPath.c_str());
		else
			fileScanner.Stop();
	}

	const char* runningText = isRunningScript ? "STOP" : "START";
	ImVec4 buttonClr = isRunningScript ? ImVec4(sf::Color(230, 40, 0)) : ImVec4(sf::Color(0, 230, 50));
	ImVec4 textClr = isRunningScript ? ImVec4(sf::Color(200, 200, 200)) : ImVec4(sf::Color(0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, buttonClr);
	ImGui::PushStyleColor(ImGuiCol_Text, textClr);
	if (ImGui::Button(runningText, ImVec2(50.0f, 50.0f)))
	{
		isRunningScript = !isRunningScript;
		// don't start scripts without an update function
		if (!script.HasUpdateFunction())
		{
			ConsoleWindow::GetInstance().HandlePrint("To 'START', you need an update function!", ConsoleWindow::LogType::error);
			//ImguiConsole::GetInstance().HandlePrint("like this:", ImguiConsole::LogType::unique);
			ConsoleWindow::GetInstance().HandlePrint("LIKE THIS:\nfunction update(dt)\n\nend", ConsoleWindow::LogType::warning);
			isRunningScript = false;
		}
		else if (isRunningScript)
		{
			if (script.HasStartFunction())
				script.InvokeStart();

			// reset mouse position to where the mouse is
			sf::Vector2i mousePos = sf::Mouse::getPosition();
			SimulateInput::GetInstance().OnMouseMoved(mousePos.x, mousePos.y);
		}
		else
		{
			// remove bindings, they will be re-attached when starting
			ScriptBinding::GetInstance().ClearBindings();
		}
	}
	ImGui::PopStyleColor(2);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(sf::Color(30, 60, 150)));
	if (ImGui::Button("Open explorer", ImVec2(100.0f, 50.0f)))
	{
		auto currentDir = FD.GetWorkingDirectory();
		currentDir.append("/Scripts");
		ShellExecute(NULL, L"open", currentDir.c_str(), NULL, NULL, SW_SHOWDEFAULT);
	}
	ImGui::PopStyleColor();

	ImGui::End();
}
