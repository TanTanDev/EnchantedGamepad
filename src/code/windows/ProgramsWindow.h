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
#include <vector>
typedef class Application Application;
typedef class FileScanner FileScanner;
typedef class Script Script;
typedef class FolderData FolderData;
typedef class ScriptBindingFileManager BindingSettings;

class ProgramsWindow
{
private:
	struct SelectedScriptFolderRef
	{
		int ScriptIndex;
		std::vector<int> recursiveFolderIndex;
	};
	SelectedScriptFolderRef selectedScriptFolderRef;

	int selectedScriptByIndex;
	
	// cashed vector we can work with every frame
	std::vector<int> currentRecursiveFolderIndex;

	int renderBrowserRecursive(FolderData* folderData, Script& fd, bool& isRunningScript, int currentSubfolderIndex, int currentFileTotalCount);
public:
	ProgramsWindow(Application& FD);
	~ProgramsWindow();

	void Render(Application& FD, FileScanner& fileScanner, Script& script, bool& isRunningScript, ScriptBindingFileManager& bindingSettings);
};