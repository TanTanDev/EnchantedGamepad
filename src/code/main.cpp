#include "SimulateInput.h"
#include "GamepadInput.h"
#include "AppSettings.h"
#include "Script.h"
#include "Timer.h"
#include "imguiThemes.h"
#include "Application.h"
#include "FileScanner.h"
#include "ImguiConsole.h"

#include <SFML/graphics.hpp>
#include <SFML/window.hpp>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <filesystem>

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
			
		}
		ImGui::SameLine();
		auto scripts = FD.GetScriptPaths();
		if (ImGui::Button("delete", ImVec2(50.5f, 30.0f)))
		{
			if (selectedScriptByIndex != -1)
				FD.DeleteFileByIndex(selectedScriptByIndex);
		}
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
			if (autoHotReload)
				fileScanner.BeginWatchDirectory(L"Scripts");
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
			ShellExecute(NULL, L"open", L"Scripts", NULL, NULL, SW_SHOWDEFAULT);
		}
		ImGui::PopStyleColor();
		fileScanner.Update(0.0f);
		ImGui::End();
		ImGui::SFML::Render(window);

		window.display();
		sf::sleep(sf::Time(sf::milliseconds(2)));
		GamepadInput::GetInstance().Update(0.05f);
		//exitApp = gamepadInput.ButtonPressed(0, GamepadInput::GAMEPAD_START);
	}
	ImGui::SFML::Shutdown();
	return 0;
}