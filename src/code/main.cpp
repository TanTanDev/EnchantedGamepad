#include "SimulateInput.h"
#include "GamepadInput.h"
#include "AppSettings.h"
#include "Script.h"
#include "Timer.h"
#include "imguiThemes.h"
#include "Application.h"
#include "FileScanner.h"
#include "ImguiConsole.h"
#include "ScriptBinding.h"

#include "ScriptBindings\EditorBinding.h"

#include <SFML/graphics.hpp>
#include <SFML/window.hpp>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <stdlib.h> // wsctombs
#include <fstream>

#include "imgui\imgui.h"
#include "imgui-sfml\imgui-SFML.h"

int main()
{
	//AppSettings appSettings;
	AppSettings::GetInstance().LoadSettingsFromScript("Scripts/globalSettings.lua");

	Script script;
	//script.Load("Scripts/test.lua");

	Timer timer;
	timer.Start();
	sf::RenderWindow window(sf::VideoMode(500, 500), "Open Game Controller");
	ImGui::SFML::Init(window);
	window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.

	sf::Vector2i mousePos = sf::Mouse::getPosition();
	SimulateInput::GetInstance().OnMouseMoved(mousePos.x, mousePos.y);
	sf::Clock deltaClock;
	SetupTheme();
	Application FD;
	FD.FindScripts("Scripts/");
	bool isRunningScript = false;
	bool autoHotReload = false;
	int selectedScriptByIndex = -1;
	FileScanner fileScanner;
	while (window.isOpen())
	{
		ImguiConsole::GetInstance().ProccessCout();
		timer.Update();
		fileScanner.Update(timer.GetDeltaTime());
		if (isRunningScript)
			script.Run(timer.GetDeltaTime());

		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			}
		}
		window.clear(sf::Color(200, 200, 200, 255));

		ImGui::SFML::Update(window, deltaClock.restart());
		ImguiConsole::GetInstance().DrawConsole(true);
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
		if (ImGui::Button("delete", ImVec2(50.5f, 30.0f))&& isAnyScriptSelected)
		{
			isRunningScript = false;
			ImGui::OpenPopup("Delete?");
		}
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(sf::Color(140,140,140)));
		if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(sf::Color(255,50,50)));
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
		ImGui::BeginChild("scriptFileList", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);

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
			if (ImGui::Button(scripts[i].c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 30)) && i != selectedScriptByIndex)
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
				ImguiConsole::GetInstance().HandlePrint("To 'START', you need an update function!", ImguiConsole::LogType::error);
				//ImguiConsole::GetInstance().HandlePrint("like this:", ImguiConsole::LogType::unique);
				ImguiConsole::GetInstance().HandlePrint("LIKE THIS:\nfunction update(dt)\n\nend", ImguiConsole::LogType::warning);
				isRunningScript = false;
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
		ImGui::Begin("Bindings");
		auto& bindings = ScriptBinding::GetInstance().GetBindings();
		ImGui::BeginChild("scriptFileList", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
		for (int i = 0; i < bindings.size(); i++)
		{
			auto bindingIt = &bindings[i];

			//float *dapointer = &bindings[i].bindingData.f;
			if (bindingIt->editType == Binding::EditType::DragFloat)
			{
				if (ImGui::DragFloat(bindingIt->GlobalName.c_str(), &bindingIt->bindingData.f, 0.01f, bindingIt->MinValue.f, bindingIt->MaxValue.f))
					ScriptBindings::SetGlobal(script.GetLuaState(), *bindingIt);
			}
			else if (bindingIt->editType == Binding::EditType::InputFloat)
			{
				if (ImGui::InputFloat(bindingIt->GlobalName.c_str(), &bindingIt->bindingData.f))
					ScriptBindings::SetGlobal(script.GetLuaState(), *bindingIt);
			}
			else if (bindingIt->editType == Binding::EditType::DragInt)
			{
				if (ImGui::DragInt(bindingIt->GlobalName.c_str(), &bindingIt->bindingData.i, 1.0f, bindingIt->MinValue.i, bindingIt->MaxValue.i))
					ScriptBindings::SetGlobal(script.GetLuaState(), *bindingIt);
			}
			else if (bindingIt->editType == Binding::EditType::InputInt)
			{
				if (ImGui::InputInt(bindingIt->GlobalName.c_str(), &bindingIt->bindingData.i))
					ScriptBindings::SetGlobal(script.GetLuaState(), *bindingIt);
			}
			else if (bindingIt->editType == Binding::EditType::SliderFloat)
			{
				if (ImGui::SliderFloat(bindingIt->GlobalName.c_str(), &bindingIt->bindingData.f, bindingIt->MinValue.f, bindingIt->MaxValue.f))
					ScriptBindings::SetGlobal(script.GetLuaState(), *bindingIt);
			}
			else if (bindingIt->editType == Binding::EditType::SliderInt)
			{
				if (ImGui::SliderInt(bindingIt->GlobalName.c_str(), &bindingIt->bindingData.i, bindingIt->MinValue.i, bindingIt->MaxValue.i))
					ScriptBindings::SetGlobal(script.GetLuaState(), *bindingIt);
			}
			else if (bindingIt->editType == Binding::EditType::ToggleBox)
			{
				if (ImGui::Checkbox(bindingIt->GlobalName.c_str(), &bindingIt->bindingData.b))
					ScriptBindings::SetGlobal(script.GetLuaState(), *bindingIt);
			}
			else if (bindingIt->editType == Binding::EditType::ToggleButton)
			{
				if (ImGui::Button(bindingIt->GlobalName.c_str()))
				{
					bindingIt->bindingData.b != bindingIt->bindingData.b;
					ScriptBindings::SetGlobal(script.GetLuaState(), *bindingIt);
				}
			}
		}
		ImGui::EndChild();
		ImGui::End();


		fileScanner.Update(0.0f);
		ImGui::SFML::Render(window);

		window.display();
		sf::sleep(sf::Time(sf::milliseconds(2)));
		GamepadInput::GetInstance().Update(0.05f);
		//exitApp = gamepadInput.ButtonPressed(0, GamepadInput::GAMEPAD_START);
	}
	ImGui::SFML::Shutdown();
	return 0;
}