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
#include "TextEditorWindow.h"
#include "../Script.h"
#include "../imgui/imgui.h"
#include "../ImguiConsole.h"

#include <fstream>
#include <iostream>

static const char* SimulateInputIdentifiers[] = { "SimulateInput","KeyUp", "KeyUp", "KeyDown", "PressKey", "SetMousePos", "MoveMouse", "ScrollMouse", "GetMousePos" };
static const char* SimulateInputDescriptions[] = { "class","[SimulateInput] arg0: key or mouse enum","[SimulateInput] arg0: key or mouse enum","[SimulateInput] arg0: key or mouse enum","[SimulateInput] arg0: Vector","[SimulateInput] arg0: Vector","[SimulateInput] arg0: number","[SimulateInput] arg0: number","[SimulateInput] return: vector" };
static const char* VectorIdentifiers[] = {"Vector", "x", "y", "Normalize", "Set", "Length", "Dot", "Zero"};
static const char* VectorDescriptions[] = { "class", "[Vector] returns: x value", "[Vector] returns: y value", "[Vector] returns: a normalized vector that has a length of 1", "[Vector] x: x value to set. y: y value to set", "[Vector] return: length of vector", "[Vector] return: (number) of the dot product", "[Vector] return: (Vector) with x/y:0"};
TextEditorWindow::TextEditorWindow()
{
	isFileLoaded = false;
	currentFileName = "";
	textEditor.SetPalette(textEditor.GetDarkPalette());
	auto lang = TextEditor::LanguageDefinition::Lua();
	for (int i = 0; i < sizeof(SimulateInputIdentifiers) / sizeof(SimulateInputIdentifiers[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = std::string(SimulateInputDescriptions[i]);
		lang.mIdentifiers.insert(std::make_pair(std::string(SimulateInputIdentifiers[i]), id));
	}
	for (int i = 0; i < sizeof(VectorIdentifiers) / sizeof(VectorIdentifiers[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = std::string(SimulateInputDescriptions[i]);
		lang.mIdentifiers.insert(std::make_pair(std::string(VectorDescriptions[i]), id));
	}
	textEditor.SetLanguageDefinition(lang);
}

TextEditorWindow::~TextEditorWindow()
{
}

void TextEditorWindow::Render(Application& FD, Script& script)
{
	ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	
	if (!(currentFileName.compare(script.GetFileName())== 0))
	{
		currentFileName = script.GetFileName();
		// attempt to load file
		std::ifstream t(currentFileName);
		if (t.good())
		{
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			textEditor.SetText(str);
			isFileLoaded = true;
		}
	}

	//ImGui::Text(script.GetFileName().c_str());
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				auto textToSave = textEditor.GetText();
				std::fstream t;
				t.open(currentFileName, std::ios::out);
				if (t.is_open())
				{
					t.write(textToSave.c_str(), textToSave.size());
					t.close();
					ImguiConsole::GetInstance().ClearLog(false);
					ImguiConsole::GetInstance().HandlePrint("--- Hot reloading ---", ImguiConsole::LogType::unique);
					script.Unload();
					script.Load(currentFileName.c_str());
				}
				else
				{
					std::cout << "Could not open file to save!\n";
				}
			}
			//if (ImGui::MenuItem("Quit", "Alt-F4"))
			//	break;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = textEditor.IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				textEditor.SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && textEditor.CanUndo()))
				textEditor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && textEditor.CanRedo()))
				textEditor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, textEditor.HasSelection()))
				textEditor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && textEditor.HasSelection()))
				textEditor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && textEditor.HasSelection()))
				textEditor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				textEditor.Paste();

			ImGui::Separator();

			//if (ImGui::MenuItem("Select all", nullptr, nullptr))
			//	textEditor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				textEditor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				textEditor.SetPalette(TextEditor::GetLightPalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	//ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
	//	editor.IsOverwrite() ? "Ovr" : "Ins",
	//	editor.CanUndo() ? "*" : " ",
	//	editor.GetLanguageDefinition().mName.c_str(), fileToEdit);
	textEditor.Render("TextEditorWindow");
	ImGui::End();
}
