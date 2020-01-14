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
#pragma once
typedef class Application Application;
typedef class FileScanner FileScanner;
typedef class Script Script;
typedef class Timer Timer;
typedef class ScriptBindingFileManager BindingSettings;
#include <SFML/Graphics.hpp>

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


class BindingsWindow
{
private:
	bool tweakWithKeyboard;
	std::vector<EditorKeyBinding> editorKeyBindings;
	std::vector<std::string> bindingFiles;
	int bindingsFileIndex;
	int bindingsFileIndexPrev;
	bool isWindowOpenPrev;
	void RenderBindingsFile(Application& FD, FileScanner& fileScanner, Script& script, Timer& timer);
	void RefreshBindingFiles(Script & script);
public:
	BindingsWindow();
	~BindingsWindow();

	void Render(Application& FD, FileScanner& fileScanner, Script& script, Timer& timer, const sf::Keyboard::Key& anyKeyPressed, ScriptBindingFileManager& bindingSettings);
};