#include "SimulateInput.h"
#include "GamepadInput.h"
#include "AppSettings.h"
#include "Script.h"
#include "Timer.h"
#include "imguiThemes.h"
#include "Application.h"

#include <SFML/graphics.hpp>
#include <SFML/window.hpp>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "imgui\imgui.h"
#include "imgui-sfml\imgui-SFML.h"

int main()
{
	//AppSettings appSettings;
	AppSettings::GetInstance().LoadSettingsFromScript("Scripts/globalSettings.lua");

	Script script;
	script.Load("Scripts/test.lua");

	Timer timer;
	timer.Start();
	sf::RenderWindow window(sf::VideoMode(500, 500), "Open Game Controller");
	ImGui::SFML::Init(window);
	window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	SimulateInput::GetInstance().OnMouseMoved(mousePos.x, mousePos.y);
	sf::Clock deltaClock;
	//SetupTheme();
	Application FD;
	while (window.isOpen())
	{
		timer.Update();
		script.Run(timer.GetDeltaTime());

		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{

				//gui.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			}
			//gui.handleEvent(event); // Pass the event to the widgets
		}
		window.clear(sf::Color(36, 50, 50, 255));

		ImGui::SFML::Update(window, deltaClock.restart());
		// Menu
		ImGui::Begin("Scripts", 0);// ImGui::SameLine();
		ImGui::Button("create new" , ImVec2(100.0f, 30.0f)); ImGui::SameLine();
		ImGui::Button("delete", ImVec2(30.5f, 30.0f));
		ImGui::Text("Files"); 
		ImGui::BeginChild("scriptFileList", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
		 
		auto directories = FD.applicationData.scriptSettings.directories;

		for (std::string g : directories)
		{
			ImGui::Button(g.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 30));
			ImGui::Selectable(g.c_str());
		}
		for (int i = 0; i < 10; i++)
		{
			//ImGui::Button("League Of Legends - Jarvan",ImVec2(ImGui::GetContentRegionAvailWidth(),30));
		}
		ImGui::EndChild();
		//if (ImGui::BeginMenuBar())
		//{
		//	if (ImGui::BeginMenu("Menu"))
		//	{
		//		ImGui::EndMenu();
		//	}
		//	if (ImGui::BeginMenu("Examples"))
		//	{
		//		ImGui::MenuItem("Main menu bar");
		//		ImGui::MenuItem("Main menu bar");
		//		ImGui::MenuItem("Main menu bar");
		//		ImGui::MenuItem("Main menu bar");
		//		ImGui::EndMenu();
		//	}
		//	if (ImGui::BeginMenu("Help"))
		//	{
		//		ImGui::MenuItem("Main menu bar");
		//		ImGui::EndMenu();
		//	}
		//	ImGui::EndMenuBar();
		//}
			ImGui::End();

		//ImGui::End(); // end window
		ImGui::SFML::Render(window);

		//gui.draw(); // Draw all widgets
		// Render
		window.display();
		sf::sleep(sf::Time(sf::milliseconds(2)));
		GamepadInput::GetInstance().Update(0.05f);
		//exitApp = gamepadInput.ButtonPressed(0, GamepadInput::GAMEPAD_START);

	}
	ImGui::SFML::Shutdown();
	return 0;
}