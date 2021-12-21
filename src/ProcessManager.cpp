#include "ProcessManager.h"

using namespace std;

#pragma region Misc Functions

ProcessManager::ProcessManager()
{
	//Constructor For Class, Do Not Remove!
}

ProcessManager::~ProcessManager()
{
	//De-Constructor
	//Clean Up! (Close Handle - Not Needed Anymore)
	CloseHandle(hProcess);
}

#pragma region Memory Functions

/* This Function Will Return A Handle To The Process So We Can Write & Read Memeory From The Process. */
void ProcessManager::Process(const char* ProcessName)
{
	//Variables
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); //Snapshot To View All Active Processes
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry); //Declare Structure Size And Populate It

	//Loop Through All Running Processes To Find Process
	do
		if (!wcscmp(ProcEntry.szExeFile, (LPWSTR)ProcessName))
		{
			//Store Process ID
			dwPID = ProcEntry.th32ProcessID;
			CloseHandle(hPID);

			//Give Our Handle All Access Rights 
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
			return;
		}
	while (Process32Next(hPID, &ProcEntry));

	cout << "You haven't even started the game yet.";
	system("pause");
	exit(0);
}

bool ProcessManager::DataCompare(BYTE* data, BYTE* sign, char* mask)
{
	for (; *mask; mask++, sign++, data++) // zzZ really?...
	{
		if (*mask == 'x' && *data != *sign)
		{
			return false;
		}
	}
	return true;
}

DWORD ProcessManager::FindSignature(DWORD base, DWORD size, BYTE* sign, char* mask)
{
	MEMORY_BASIC_INFORMATION mbi = {0};
	DWORD offset = 0;
	while (offset < size) // TODO: while still in module
	{
		VirtualQueryEx(hProcess, (LPCVOID)(base + offset), &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		if (mbi.State != MEM_FREE)
		{
			BYTE* buffer = new BYTE[mbi.RegionSize];
			ReadProcessMemory(hProcess, mbi.BaseAddress, buffer, mbi.RegionSize, NULL);
			for (int i = 0; i < mbi.RegionSize; i++)
			{
				if (DataCompare(buffer + i, sign, mask))
				{
					delete[] buffer;
					return (DWORD)mbi.BaseAddress + i;
				}
			}

			delete[] buffer;
		}
		offset += mbi.RegionSize;
	}
	return 0;
}


/* Returns The Base Address Of The Specified Module Inside The Target Process
/* e.g.[ Module("client.dll"); ]. */
MODULEENTRY32 ProcessManager::Module(const char* ModuleName)
{
	//Variables
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	//Take A Module Snapshot Of The Process (Grab All Loaded Modules)
	MODULEENTRY32 mEntry; //Declare Module Entry Structure
	mEntry.dwSize = sizeof(mEntry); //Declare Structure Size And Populate It With Loaded Modules ??? why ???

	//Scan For Module By Name
	do
          if (!wcscmp(mEntry.szModule, (LPWSTR)ModuleName))
		{
			CloseHandle(hModule);
			return mEntry;
		}
	while (Module32Next(hModule, &mEntry));

	throw "Could not find module, retrying...";
}

DWORD ProcessManager::FindAddress(DWORD mod, DWORD modsize, BYTE* sig, const char* mask, scandefintions_t def)
{
	if (def == scandefintions_t::read)
	{
		DWORD initAddress = FindSignature(mod, modsize, sig, (char*)mask);
		DWORD ptrAddress = Read<DWORD>(initAddress);
		return ptrAddress - mod;
	}

	if (def == scandefintions_t::subtract)
          return (FindSignature(mod, modsize, sig, (char*)mask)) - mod;

	if (def == scandefintions_t::none)
          return FindSignature(mod, modsize, sig, (char*)mask);
}

// TODO: for reading with extra offset?
DWORD ProcessManager::FindAddress(DWORD mod, DWORD modsize, BYTE* sig, const char* mask, scandefintions_t def,
                                  int extra)
{
	if (def == scandefintions_t::read)
	{
    DWORD initAddress = FindSignature(mod, modsize, sig, (char*)mask);
		DWORD ptrAddress = Read<DWORD>(initAddress + extra);
		return ptrAddress - mod;
	}
}
#pragma endregion
