#include "ProcessManager.h"

using namespace std;

ProcessManager::ProcessManager()
{
}

ProcessManager::~ProcessManager()
{
	CloseHandle(hProcess);
}


/* This Function Will Return A Handle To The process So We Can Write & read Memeory From The process. */
void ProcessManager::process(LPCWSTR ProcessName)
{
	//Variables
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); //Snapshot To View All Active Processes
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry); //Declare Structure Size And Populate It

	//Loop Through All Running Processes To Find process
	do
		if (!wcscmp(ProcEntry.szExeFile, ProcessName))
		{
			//Store process ID
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

bool ProcessManager::pataCompare(BYTE* data, BYTE* sign, char* mask)
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

DWORD ProcessManager::findSignature(DWORD base, DWORD size, BYTE* sign, char* mask)
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
			for (SIZE_T i = 0; i < mbi.RegionSize; i++)
			{
				if (pataCompare(buffer + i, sign, mask))
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


/* Returns The Base Address Of The Specified module Inside The Target process
/* e.g.[ module("client.dll"); ]. */
MODULEENTRY32 ProcessManager::module(LPCWSTR ModuleName) {
	//Variables
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	//Take A module Snapshot Of The process (Grab All Loaded Modules)
	MODULEENTRY32 mEntry; //Declare module Entry Structure
	mEntry.dwSize = sizeof(mEntry); //Declare Structure Size And Populate It With Loaded Modules ??? why ???

	//Scan For module By Name
	do
		if (!wcscmp(mEntry.szModule, ModuleName))
		{
			CloseHandle(hModule);
			return mEntry;
		}
	while (Module32Next(hModule, &mEntry));

	throw "Could not find module, retrying...";
}

DWORD ProcessManager::findAddress(DWORD mod, DWORD modsize, BYTE* sig, const char* mask, scandefintions_t def)
{
	if (def == scandefintions_t::read)
	{
		DWORD initAddress = findSignature(mod, modsize, sig, (char*)mask);
		DWORD ptrAddress = read<DWORD>(initAddress);
		return ptrAddress - mod;
	}

	if (def == scandefintions_t::subtract)
		return (findSignature(mod, modsize, sig, (char*)mask)) - mod;

	if (def == scandefintions_t::none)
		return findSignature(mod, modsize, sig, (char*)mask);
}

// TODO: for reading with extra offset?
DWORD ProcessManager::findAddress(DWORD mod, DWORD modsize, BYTE* sig, const char* mask, scandefintions_t def,
                                  int extra)
{
	if (def == scandefintions_t::read)
	{
		DWORD initAddress = findSignature(mod, modsize, sig, (char*)mask);
		DWORD ptrAddress = read<DWORD>(initAddress + extra);
		return ptrAddress - mod;
	}
}


template <class cData>
cData ProcessManager::read(DWORD dwAddress) {
  // TODO: This uses the template class to dynamically decide the data type to
  // read.
  cData cRead;  // Generic Variable To Store Data
  ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cRead, sizeof(cData), NULL);
  // Win API - Reads Data At Specified Location
  return cRead;  // Returns Value At Specified dwAddress
}

template <class cData>
cData ProcessManager::readString(DWORD dwAddress) {
  cData csRead;  // Generic Variable To Store Data
  ReadProcessMemory(hProcess, (LPVOID)dwAddress, &csRead, 32 * sizeof(cData),
                    NULL);
  // Win API - Reads Data At Specified Location
  return csRead;  // Returns Value At Specified dwAddress
}

