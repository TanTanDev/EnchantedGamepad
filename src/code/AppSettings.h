#pragma once

class AppSettings
{
public:
	int ExitAppKey;

	AppSettings();
	~AppSettings();

	bool LoadSettingsFromScript(const char* fileName);
};