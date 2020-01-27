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
#include "BindingsWindow.h"

#include "../imgui/imgui.h"
#include "../ScriptBinding.h"
#include "../ScriptBindings/EditorBinding.h"
#include "../Application.h"
#include "../FileScanner.h"
#include "../Script.h"
#include "../Windows/ConsoleWindow.h"
#include "../imguiExtensions/imguiExtensions.h"
#include "../ScriptBindingFileManager.h"

#include "../SfKeyToName.h"

BindingsWindow::BindingsWindow()
	:bindingsFileIndex(0)
{
	tweakWithKeyboard = false;
	isWindowOpenPrev = false;
	bindingFiles.push_back("FakeName.Binding");
	bindingFiles.push_back("Jarvan.Binding");
	bindingFiles.push_back("Singed.binding");
	bindingFiles.push_back("kled.binding");
}

BindingsWindow::~BindingsWindow()
{
}

void BindingsWindow::Render(Application & FD, FileScanner & fileScanner, Script & script, Timer& timer, const sf::Keyboard::Key& anyKeyPressed)
{
	bool isWindowOpen = ImGui::Begin("Bindings");
	if (!script.IsGood())
	{
		ImGui::End();
		return;
	}
	RenderSaveAs(script);
	RenderRemovePopup(script);

	// Window opened
	if (isWindowOpen && !isWindowOpenPrev)
	{
		//const std::string selectedScriptFileName = script.GetFileName();
	//	bindingSettings.ApplyBindings(script);
		RefreshBindingFiles(script);
	}
	isWindowOpenPrev = isWindowOpen;


	if (isWindowOpen)
	{
		ImGui::Checkbox("Tweakable with keyboard", &tweakWithKeyboard);

		RenderBindingsFile(FD, fileScanner, script, timer);

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
					else if (bindings[i].bindingDataType == Binding::BindingDataType::Int)
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
	}
	ImGui::End();
}

void BindingsWindow::RenderBindingsFile(Application & FD, FileScanner & fileScanner, Script & script, Timer & timer)
{
	ImGui::Combo("File Bindings", &bindingsFileIndex, bindingFiles);
	if (bindingsFileIndex != bindingsFileIndexPrev)
	{
		ScriptBindingFileManager::GetInstance().ApplyBindings(script, bindingFiles[bindingsFileIndex]);
	}
	bindingsFileIndexPrev = bindingsFileIndex;
	bool isUntiteled = !(GetCurrentBindingFileName().compare("UNTITELED") == 0);

	if (!isUntiteled)
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha*0.5f);
	if (ImGui::Button("Save"))
	{
		std::string currentBindingFileName = GetCurrentBindingFileName();
		ScriptBindingFileManager::GetInstance().CreateNewBindingFileAndSave(currentBindingFileName);
		ScriptBindingFileManager::GetInstance().SaveGlobalToFile();
	}
	ImGui::SameLine();
	if (!isUntiteled)
		ImGui::PopStyleVar();
	if (ImGui::Button("Save As"))
		ImGui::OpenPopup("Save As");
	ImGui::SameLine();
	if (!isUntiteled)
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha*0.5f);
	if (ImGui::Button("Delete") && isUntiteled)
		ImGui::OpenPopup("Delete File");
	if (!isUntiteled)
		ImGui::PopStyleVar();

	ImGui::SameLine();
	ImGui::Button("Explore");
	// get latest file for bindings, if non existant, construct an empty one based on the lua state
	// assign the lua state globals from the file json
	// maybe make an external class that handles this...
}

void BindingsWindow::RenderSaveAs(Script & script)
{
	if (!ImGui::BeginPopupModal("Save As", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		return;

	ImGui::Text("Save bindings file as name: ");
	ImGui::Separator();
	static char inputFileNameBuf[32] = "";
	ImGui::InputText("+ .binding", inputFileNameBuf, 32);
	if (ImGui::Button("OK", ImVec2(120, 0)))
	{
		std::string bindingFileName = "Bindings/" + std::string(inputFileNameBuf) + ".binding";
		ScriptBindingFileManager::GetInstance().CreateNewBindingFileAndSave(bindingFileName);
		ScriptBindingFileManager::GetInstance().AddNewBindingToGlobal(script.GetFileName(), bindingFileName);
		ScriptBindingFileManager::GetInstance().SaveGlobalToFile();
		RefreshBindingFiles(script);
		ImGui::CloseCurrentPopup();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
	ImGui::EndPopup();
}

void BindingsWindow::RenderRemovePopup(Script & script)
{
	if (!ImGui::BeginPopupModal("Delete File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		return;
	std::string currentBindingFileName = GetCurrentBindingFileName();
	//const char* titleMsg = std::string("YOU ARE ABOUT TO DELETE FILE:")
	//	.append(currentBindingFileName).c_str();

	ImGui::Text("YOU ARE ABOUT TO DELETE FILE:");
	ImGui::Text(currentBindingFileName.c_str());
	ImGui::Separator();
	if (ImGui::Button("DELETE", ImVec2(120, 0)))
	{

		ScriptBindingFileManager::GetInstance().RemoveBindingAndSaveGlobal(script.GetFileName(), currentBindingFileName);
		bindingsFileIndex--;
		if (bindingsFileIndex < 0)
			bindingsFileIndex = 0;
		RefreshBindingFiles(script);
		ImGui::CloseCurrentPopup();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
	ImGui::EndPopup();
}

void BindingsWindow::RefreshBindingFiles(Script & script)
{
	bindingFiles.clear();
	std::vector<std::string> refreshedBindingFiles = ScriptBindingFileManager::GetInstance().GetBindingFileNames(script.GetFileName());
	for (int i = 0; i < refreshedBindingFiles.size(); i++)
	{
		bindingFiles.push_back(refreshedBindingFiles[i]);
	}
}

void BindingsWindow::ApplyBindings(Script & script)
{
	std::string bindingFileName = GetCurrentBindingFileName();
	if (bindingFileName.compare("") != 0)
		ScriptBindingFileManager::GetInstance().ApplyBindings(script, bindingFileName);
}

std::string BindingsWindow::GetCurrentBindingFileName()
{
	if (bindingsFileIndex < 0 || bindingsFileIndex > bindingFiles.size())
		return std::string();
	return bindingFiles[bindingsFileIndex];
}

void BindingsWindow::HandleOnScriptChanged()
{
}
