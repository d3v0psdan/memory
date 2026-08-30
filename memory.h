#pragma once

#include <Windows.h>
#include <TlHelp32.h>

namespace memory
{
	inline bool IsValidHandle(HANDLE value)
	{
		return (value != nullptr && value != INVALID_HANDLE_VALUE);
	}

	class Process
	{
	private:
		DWORD mProcessId;
		HANDLE mProcessHandle;

		void Reset()
		{
			mProcessId = 0;
			mProcessHandle = nullptr;
		}
	public:
		Process() {}
		~Process() { this->Reset(); }

		bool Attach(const char* processName)
		{
			HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (!IsValidHandle(processSnapshot))
				return false;

			PROCESSENTRY32 processEntry;
			processEntry.dwSize = sizeof(PROCESSENTRY32);

			if (!Process32First(processSnapshot, &processEntry))
			{
				CloseHandle(processSnapshot);
				return false;
			}

			do
			{
				if (strcmp(processEntry.szExeFile, processName) == 0)
				{
					mProcessId = processEntry.th32ProcessID;
					CloseHandle(processSnapshot);
					return true;
				}
			} while (Process32Next(processSnapshot, &processEntry));

			CloseHandle(processSnapshot);
			return false;
		}
	};

	class Memory
	{
	private:
		Process mProcess;
	public:
		Memory() {}
		~Memory(){};

		template<typename Type>
		Type Write(PVOID address, PVOID  buffer, SIZE_T size = sizeof(Type))
		{
			Type bytesWritten = 0;
			if (!IsValidHandle(mProcess.mProcessHandle))
				return bytesWritten;

			WriteProcessMemory(mProcess.mProcessHandle, address, buffer, size, &bytesWritten);
			return bytesWritten;
		}

		template<typename Type>
		Type Read(PVOID address, PVOID buffer, SIZE_T size = sizeof(Type))
		{
			Type bytesRead = 0;
			if (!IsValidHandle(mProcess.mProcessHandle))
				return bytesRead;

			ReadProcessMemory(mProcess.mProcessHandle, address, buffer, size, &bytesRead);
			return bytesRead;
		}
	};
}

