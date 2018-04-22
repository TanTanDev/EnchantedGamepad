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
#include "Singleton.h"
#include "Timer.h"
#include <windows.h>
#include <WinBase.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#include <thread>
#include <vector>

#define MAX_DIRS 25
#define MAX_FILES 255
#define MAX_BUFFER 4096

class Timer;

class FileScanner
{
private:
	typedef struct _DIRECTORY_INFO {
		HANDLE hDir;
		TCHAR lpszDirName[MAX_PATH];
		CHAR lpBuffer[MAX_BUFFER];
		DWORD dwBufLength;
		OVERLAPPED Overlapped;
	}DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;

	DIRECTORY_INFO DirInfo[MAX_DIRS];
	HANDLE exitEvent;
	//OVERLAPPED PollingOverlap;
	//DWORD nRet;
	//char buf[2048];

	bool isWatching;
	void watchDirectory(LPCWSTR path);
	std::thread fileScanThread;
	std::vector<std::string> changedFiles;

	Timer pollTimer;
	bool isPollReady;
	float timeBetweenPolls;
public:
	void BeginWatchDirectory(LPCWSTR path);
	void Stop();
	void Update(float dt);

	const std::vector<std::string> PollChanges();
	const bool& IsPollReady();
	FileScanner();
	~FileScanner();
};