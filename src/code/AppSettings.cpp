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

#include "AppSettings.h"
#include "Script.h"
#include "Rect.h"

#include <Windows.h>
#include <iostream>
AppSettings::AppSettings()
	:ExitAppKey(0)
{
	screenResolutionHeight = GetSystemMetrics(SM_CYSCREEN);
	screenResolutionWidth = GetSystemMetrics(SM_CXSCREEN);
}

AppSettings::~AppSettings()
{
}

bool AppSettings::LoadSettingsFromScript(const char * fileName)
{
	Script settingsScript;
	settingsScript.Load(fileName);

	if (settingsScript.Get("ExitAppKey", ExitAppKey))
	{
		return true;
	}

	ExitAppKey = 155;
	std::cout << "ExitAppKey, wasn't set in: " << fileName << "Defaulting it to: F8" << std::endl;
	return false;
}

Rect AppSettings::GetWindowRectByName(const char* name)
{
	HWND windowHandle = FindWindowA(0, name);
	RECT outRect;
	bool isWindowAvaliable = GetWindowRect(windowHandle, &outRect);
	// if window is fullscreen, we might get weird rect values
	bool windowRectFullscreen = (outRect.bottom < 0 || outRect.top < 0 || outRect.left < 0 || outRect.right < 0);
	if(isWindowAvaliable || windowRectFullscreen)
	{
		outRect.top = 0;
		outRect.left = 0;
		outRect.bottom = screenResolutionHeight;
		outRect.right = screenResolutionWidth;
	}

	Rect newRect;
	newRect.X = outRect.left;
	newRect.Y = outRect.top;
	newRect.Width = outRect.right - outRect.left;
	newRect.Height = outRect.bottom - outRect.top;
	return newRect;
}
