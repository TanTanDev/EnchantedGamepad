#include "FileScanner.h"

#include <iostream>
#include <thread>


void FileScanner::BeginWatchDirectory(LPCWSTR path)
{
	isWatching = true;
	fileScanThread = std::thread(&FileScanner::watchDirectory, this, path);
}

void FileScanner::watchDirectory(LPCWSTR path)
{
	char buf[2048];
	DWORD nRet;
	BOOL result = TRUE;
	char filename[MAX_PATH];
	DirInfo[0].hDir = CreateFile(path, GENERIC_READ | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL);

	if (DirInfo[0].hDir == INVALID_HANDLE_VALUE)
	{
		return; //cannot open folder
	}

	lstrcpy(DirInfo[0].lpszDirName, path);
	OVERLAPPED PollingOverlap;

	FILE_NOTIFY_INFORMATION* pNotify;
	int offset;
	PollingOverlap.OffsetHigh = 0;
	PollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	while (result)
	{
		pollTimer.Start();
		result = ReadDirectoryChangesW(
			DirInfo[0].hDir,// handle to the directory to be watched
			&buf,// pointer to the buffer to receive the read results
			sizeof(buf),// length of lpBuffer
			TRUE,// flag for monitoring directory or directory tree
			FILE_NOTIFY_CHANGE_FILE_NAME |
			//FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_SIZE,
			//FILE_NOTIFY_CHANGE_LAST_WRITE,
			//FILE_NOTIFY_CHANGE_LAST_ACCESS |
			//FILE_NOTIFY_CHANGE_CREATION,
			&nRet,// number of bytes returned
			&PollingOverlap,// pointer to structure needed for overlapped I/O
			NULL);
		HANDLE handles[2];
		handles[0] = PollingOverlap.hEvent;
		handles[1] = exitEvent;
		WaitForMultipleObjects(2, handles, FALSE, INFINITE);

		pollTimer.Reset();

		// User tried to cancel, don't lookup changed data
		if (!isWatching)
		{
			CloseHandle(DirInfo[0].hDir);
			return;
		}
		offset = 0;
		int rename = 0;
		char oldName[260];
		char newName[260];
		do
		{
			pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
			strcpy(filename, "");
			int filenamelen = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / 2, filename, sizeof(filename), NULL, NULL);
			filename[pNotify->FileNameLength / 2] = ' ';

			std::string changedFileName = std::string(filename).substr(0, filenamelen);
			bool isDuplicate = false;
			for (auto file : changedFiles)
			{
				// skip same filename
				if (file == changedFileName)
					isDuplicate = true;
			}
			if (!isDuplicate)
				changedFiles.push_back(changedFileName);
			offset += pNotify->NextEntryOffset;

		} while (pNotify->NextEntryOffset); //(offset != 0);
	}
	CloseHandle(DirInfo[0].hDir);
}

void FileScanner::Stop()
{
	bool wasWatching = isWatching;
	isWatching = false;
	if (wasWatching)
	{
		SetEvent(exitEvent);
		ResetEvent(exitEvent);
		fileScanThread.join();
		changedFiles.empty();
	}
}

void FileScanner::Update(float dt)
{
	pollTimer.Update();
	if (pollTimer.GetElapsedTime() > timeBetweenPolls)
	{
		isPollReady = true;
	}
}

const std::vector<std::string> FileScanner::PollChanges()
{
	if (!isPollReady)
	{
		return std::vector<std::string>();
	}
	isPollReady = false;
	pollTimer.Reset();
	pollTimer.Stop();
	auto copy = changedFiles;
	changedFiles.clear();
	return copy;
}

const bool & FileScanner::IsPollReady()
{
	return isPollReady;
}

FileScanner::FileScanner()
{
	isWatching = false;
	timeBetweenPolls = 0.3f;
	changedFiles.reserve(2);
	exitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

FileScanner::~FileScanner()
{
	Stop();
}
