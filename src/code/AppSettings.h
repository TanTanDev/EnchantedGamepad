#pragma once
#include "Singleton.h"

class AppSettings : public Singleton<AppSettings>
{
public:
	int ExitAppKey;
	int screenResolutionWidth;
	int screenResolutionHeight;

	AppSettings();
	~AppSettings();

	bool LoadSettingsFromScript(const char* fileName);
};