#include "AppSettings.h"
#include "Script.h"

#include <iostream>
AppSettings::AppSettings()
	:ExitAppKey(0)
{
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
