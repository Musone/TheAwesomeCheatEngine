#include "Process.h"

using namespace std;

Process::Process()
{
}

Process::~Process()
{
	CloseHandle(hProcess);
}


/* This Function Will Return A Handle To The process So We Can Write & read Memeory From The process. */
void Process::process(LPCWSTR ProcessName)
{
	while (1)
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
				cout << endl;
				return;
			}
		while (Process32Next(hPID, &ProcEntry));

		cout << "Start the game niga\n";
		Sleep(5000);
	}
}

bool Process::sigEqual(BYTE* data, BYTE* sig, char* mask)
{
	// todo: there is no check here for if the the data pointer exceeds the region's bounds... I think it still works
	// todo: because the region is alligned to the # of bytes in our signature... (aligned to 12 bytes?... or just luck...)
	for (; *mask; mask++, sig++, data++) // zzZ really?... actually this is really smart lol...
	{
		// todo: this is iterating over the mask checking for wildcards...
		// todo: *mask is a null terminated string, so when the byte is 0, we know its the end. genius
		if (*mask == 'x' && *data != *sig)
		{
			return false;
		}
	}
	return true;
}

DWORD Process::findSignature(DWORD base, DWORD size, BYTE* sig, char* mask)
{
	MEMORY_BASIC_INFORMATION mbi = {0}; // I think this is init'ing with 0's
	DWORD offset = 0;

	while (offset < size) // TODO: while still in module
	{
		VirtualQueryEx(hProcess, (LPCVOID)(base + offset), &mbi, sizeof(MEMORY_BASIC_INFORMATION));


		if (mbi.State != MEM_FREE)
		{
			// todo: does region represent a page??? What exactly is a "region"...
			BYTE* buffer = new BYTE[mbi.RegionSize];
			ReadProcessMemory(hProcess, mbi.BaseAddress, buffer, mbi.RegionSize, NULL);


			for (SIZE_T i = 0; i < mbi.RegionSize; i++)
			{
				// todo: iterates over the entire region, checking if signature matches.
				if (sigEqual(buffer + i, sig, mask))
				{
					delete[] buffer;
					// todo: returns the virtual address of the signature
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
MODULEENTRY32 Process::module(LPCWSTR ModuleName)
{
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

// TODO: for reading with extra offset? I am suspicious to the integrity of the offsets this method is returning...
DWORD Process::findAddress(DWORD mod, DWORD modsize, BYTE* sig, const char* mask, int extra)
{
	//todo: what is extra???? Why do we need extra??? What the fuck is this???

	DWORD initAddress = findSignature(mod, modsize, sig, (char*)mask);

	if (!initAddress)
	{
		throw "could not find signature";
	}

	// todo: modify how we are finding and overriding signatures.
	DWORD ptrAddress = read<DWORD>(initAddress + extra);
	return ptrAddress - mod; // todo: this is the offset
}


void Process::writeAddress(DWORD addr, BYTE* buff, SIZE_T nBytes)
{
	// cout << "#bytes " << nBytes << endl;
	if (!WriteProcessMemory(hProcess, (LPVOID)addr, buff, nBytes, NULL))
		throw "(Process/writeAddress) Could not write to memory process";
}


template <class cData>
cData Process::read(DWORD dwAddress)
{
	// TODO: This uses the template class to dynamically decide the data type to
	// read.
	cData cRead; // Generic Variable To Store Data
	ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cRead, sizeof(cData), NULL);
	// Win API - Reads Data At Specified Location
	return cRead; // Returns Value At Specified dwAddress
}

void Process::readAddress(DWORD addr, BYTE* buff, SIZE_T nBytes)
{
	if (!ReadProcessMemory(hProcess, (LPVOID)addr, buff, nBytes, NULL))
		throw "(Process/writeAddress) Could not write to memory process";
}


template <class cData>
cData Process::readString(DWORD dwAddress)
{
	cData csRead; // Generic Variable To Store Data
	ReadProcessMemory(hProcess, (LPVOID)dwAddress, &csRead, 32 * sizeof(cData),
	                  NULL);
	// Win API - Reads Data At Specified Location
	return csRead; // Returns Value At Specified dwAddress
}
