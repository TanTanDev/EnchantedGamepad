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


// unknown is not included
static const char* SF_KEY_NAMES[sf::Keyboard::KeyCount] =
{
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Num0",
	"Num1",
	"Num2",
	"Num3",
	"Num4",
	"Num5",
	"Num6",
	"Num7",
	"Num8",
	"Num9",
	"Escape",
	"LControl",
	"LShift",
	"LAlt",
	"LSystem",
	"RControl",
	"RShift",
	"RAlt",
	"RSystem",
	"Menu",
	"LBracket",
	"RBracket",
	"SemiColon",
	"Comma",
	"Period",
	"Quote",
	"Slash",
	"BackSlash",
	"Tilde",
	"Equal",
	"Dash",
	"Space",
	"Return",
	"BackSpace",
	"Tab",
	"PageUp",
	"PageDown",
	"End",
	"Home",
	"Insert",
	"Delete",
	"Add",
	"Subtract",
	"Multiply",
	"Divide",
	"Left",
	"Right",
	"Up",
	"Down",
	"Numpad0",
	"Numpad1",
	"Numpad2",
	"Numpad3",
	"Numpad4",
	"Numpad5",
	"Numpad6",
	"Numpad7",
	"Numpad8",
	"Numpad9",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"Pause"
};

// used to controll editor bindings through the keyboard
struct EditorKeyBinding
{
	EditorKeyBinding()
		:KeyIncrease(sf::Keyboard::Key::Unknown),
		KeyDecrease(sf::Keyboard::Key::Unknown),
		IsWaintingForKeyDecreaseInput(false),
		IsWaintingForKeyIncreaseInput(false)
	{}
	const bool IsWaitingForInput()
	{
		return IsWaintingForKeyIncreaseInput || IsWaintingForKeyDecreaseInput;
	}
	bool IsWaintingForKeyIncreaseInput;
	bool IsWaintingForKeyDecreaseInput;
	sf::Keyboard::Key KeyIncrease;
	sf::Keyboard::Key KeyDecrease;
};

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
	window.setKeyRepeatEnabled(false);
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	SimulateInput::GetInstance().OnMouseMoved(mousePos.x, mousePos.y);
	sf::Clock deltaClock;
	SetupTheme();
	Application FD;
	//Rect da = FD.GetWindowRectByName("Kalkylatorn");
	//SimulateInput::GetInstance().SetMousePos(da.X+da.Width*0.5f,da.Y+da.Height*0.5f);
	FD.FindScripts("Scripts/");
	bool isRunningScript = false;
	bool autoHotReload = false;
	bool tweakWithKeyboard = false;
	int selectedScriptByIndex = -1;
	FileScanner fileScanner;

	sf::Keyboard::Key anyKeyPressed = sf::Keyboard::Key::Unknown;

	std::vector<EditorKeyBinding> editorKeyBindings;
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
		//if (GetKeyState('A') & 0x8000/*check if high-order bit is set (1 << 15)*/)
		//{
		//	SimulateInput::GetInstance().MoveMouseDelta(1, 0, (DWORD)timer.GetDeltaTime()*10.0f);
		//}
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
					if (script.HasStartFunction())
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
			else if (isRunningScript)
			{
				if(script.HasStartFunction())
					script.InvokeStart();

				// reset mouse position to where the mouse is
				sf::Vector2i mousePos = sf::Mouse::getPosition();
				SimulateInput::GetInstance().OnMouseMoved(mousePos.x, mousePos.y);
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
		ImGui::Checkbox("Tweakable with keyboard", &tweakWithKeyboard);

		auto& bindings = ScriptBinding::GetInstance().GetBindings();

		if (tweakWithKeyboard)
			ImGui::Columns(2);

		ImGui::BeginChild("scriptFileList", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
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

		// todo: more elegant rebuilding of keybinding data, when script is loaded
		if (bindings.size() != editorKeyBindings.size())
		{
			std::cout << "rebuilding editor key bindings\n";
			editorKeyBindings.clear();
			editorKeyBindings.reserve(bindings.size());
			for (int i = 0; i < bindings.size(); i++)
				editorKeyBindings.push_back(EditorKeyBinding());
		}

		ImGui::EndChild();
		if (tweakWithKeyboard)
		{
			ImGui::NextColumn();
			ImGui::BeginChild("scriptFileListTweakBinding", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
			for (int i = 0; i < bindings.size(); i++)
			{
				EditorKeyBinding& currentKeyBinding = editorKeyBindings[i];
				// Input
				if (currentKeyBinding.IsWaintingForKeyDecreaseInput)
				{
					if (anyKeyPressed != sf::Keyboard::Key::Unknown)
					{
						currentKeyBinding.KeyDecrease = anyKeyPressed;
						currentKeyBinding.IsWaintingForKeyDecreaseInput = false;
					}
				}
				else if (sf::Keyboard::isKeyPressed(currentKeyBinding.KeyDecrease))
				{
					if (bindings[i].bindingDataType == Binding::BindingDataType::Float)
					{
						bindings[i].bindingData.f -= 0.5f*(bindings[i].MaxValue.f - bindings[i].MinValue.f)*timer.GetDeltaTime();
						ScriptBindings::SetGlobal(script.GetLuaState(), bindings[i]);
					}
					else if (bindings[i].bindingDataType == Binding::BindingDataType::Int)
						bindings[i].bindingData.i -= 1;
				}
				if (currentKeyBinding.IsWaintingForKeyIncreaseInput)
				{
					if (anyKeyPressed != sf::Keyboard::Key::Unknown)
					{
						currentKeyBinding.KeyIncrease = anyKeyPressed;
						currentKeyBinding.IsWaintingForKeyIncreaseInput = false;
					}
				}
				else if (sf::Keyboard::isKeyPressed(currentKeyBinding.KeyIncrease))
				{
					if (bindings[i].bindingDataType == Binding::BindingDataType::Float)
					{
						bindings[i].bindingData.f += 0.5f*(bindings[i].MaxValue.f - bindings[i].MinValue.f)*timer.GetDeltaTime();
						ScriptBindings::SetGlobal(script.GetLuaState(), bindings[i]);
					}
					else if(bindings[i].bindingDataType == Binding::BindingDataType::Int)
						bindings[i].bindingData.i += 1;
				}

				// GUI
				const char* decreaseButtonText;
				const char* increaseButtonText;
				if (currentKeyBinding.IsWaintingForKeyDecreaseInput)
				{
					decreaseButtonText = " ... ";
				}
				else
				{
					bool hasDecreaseKey = (editorKeyBindings[i].KeyDecrease != sf::Keyboard::Unknown);
					decreaseButtonText = (hasDecreaseKey) ? SF_KEY_NAMES[editorKeyBindings[i].KeyDecrease] : "  -  ";
				}

				if (currentKeyBinding.IsWaintingForKeyIncreaseInput)
				{
					increaseButtonText = " ... ";
				}
				else
				{
					bool hasIncreaseKey = (editorKeyBindings[i].KeyIncrease != sf::Keyboard::Unknown);
					increaseButtonText = (hasIncreaseKey) ? SF_KEY_NAMES[editorKeyBindings[i].KeyIncrease] : "  +  ";
				}

				ImGui::PushID(i);
				if (ImGui::Button(decreaseButtonText))
				{
					currentKeyBinding.IsWaintingForKeyDecreaseInput = !currentKeyBinding.IsWaintingForKeyDecreaseInput;
				}
				ImGui::PopID();
				ImGui::SameLine();
				ImGui::PushID(i * 999 + 1);
				if (ImGui::Button(increaseButtonText))
				{
					currentKeyBinding.IsWaintingForKeyIncreaseInput = !currentKeyBinding.IsWaintingForKeyIncreaseInput;
				}
				ImGui::PopID();
			}
			ImGui::EndChild();
		}
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