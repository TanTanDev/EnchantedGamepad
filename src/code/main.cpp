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

#include "windows\ProgramsWindow.h"
#include "windows\SelectedScriptWindow.h"
#include "windows\BindingsWindow.h"

// REMOVe
#include "Rect.h"

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
	AppSettings::GetInstance().LoadSettingsFromScript("Scripts/globalSettings.lua");

	Script script;

	Timer timer;
	timer.Start();
	sf::RenderWindow window(sf::VideoMode(500, 500), "Open Game Controller");
	ImGui::SFML::Init(window);
	window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
	window.setKeyRepeatEnabled(false);
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	SimulateInput::GetInstance().OnMouseMoved(mousePos.x, mousePos.y);
	sf::Clock deltaClock;
	SetupTheme();
	Application app;
	
	// windows
	ProgramsWindow programsWindow;
	SelectedScriptWindow selectedScriptWindow;
	BindingsWindow bindingWindow;

	app.FindScripts("Scripts/");
	bool isRunningScript = false;
	bool autoHotReload = false;
	bool tweakWithKeyboard = false;
	int selectedScriptByIndex = -1;
	FileScanner fileScanner;

	sf::Keyboard::Key anyKeyPressed = sf::Keyboard::Key::Unknown;

	while (window.isOpen())
	{
		ImguiConsole::GetInstance().ProccessCout();
		timer.Update();
		fileScanner.Update(timer.GetDeltaTime());
		if (isRunningScript)
			script.InvokeUpdate(timer.GetDeltaTime());

		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			}
			if (event.type == sf::Event::KeyPressed)
			{
				anyKeyPressed = event.key.code;
			}
			else if(event.type == sf::Event::KeyReleased)
				anyKeyPressed = sf::Keyboard::Key::Unknown;
		}
		window.clear(sf::Color(200, 200, 200, 255));

		ImGui::SFML::Update(window, deltaClock.restart());
		ImguiConsole::GetInstance().DrawConsole(true);
	
		// window renders
		programsWindow.Render(app, fileScanner, script, isRunningScript);
		selectedScriptWindow.Render(app, fileScanner, script, isRunningScript, autoHotReload);
		bindingWindow.Render(app, fileScanner, script, timer, anyKeyPressed);

		fileScanner.Update(0.0f);
		ImGui::SFML::Render(window);

		window.display();
		sf::sleep(sf::Time(sf::milliseconds(2)));
		GamepadInput::GetInstance().Update(timer.GetDeltaTime());
		//exitApp = gamepadInput.ButtonPressed(0, GamepadInput::GAMEPAD_START);
	}
	ImGui::SFML::Shutdown();
	return 0;
}