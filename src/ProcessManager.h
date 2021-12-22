#ifndef PROCMEM_H
#define PROCMEM_H

#define WIN32_LEAN_AND_MEAN //Excludes Headers We Wont Use (Increase Compile Time)

#include <Windows.h> //Standard Windows Functions/Data Types
#include <iostream> //Constains Input/Output Functions (cin/cout etc..)
#include <TlHelp32.h> //Contains read/Write Functions
#include <string> //Support For Strings
#include <sstream> //Supports Data Conversion

#pragma warning(disable : 4996)

enum scandefintions_t
{
	none,
	read,
	subtract
};

class ProcessManager
{
	// todo: refactor the process manager
protected:
	//STORAGE
	HANDLE hProcess;
	DWORD dwPID, dwProtection, dwCaveAddress;

	//MISC
	BOOL bPOn, bIOn, bProt;

public:
	ProcessManager();
	~ProcessManager();

	//READ MEMORY
	template <class cData>
	cData read(DWORD dwAddress);
	template <class cData>
	cData readString(DWORD dwAddress);

	void process(LPCWSTR ProcessName); //Return Handle To The process
    MODULEENTRY32 module(LPCWSTR ModuleName);  // Return module Base Address

	DWORD findAddress(DWORD mod, DWORD modsize, BYTE* sig, const char* mask, scandefintions_t def);
	DWORD findAddress(DWORD mod, DWORD modsize, BYTE* sig, const char* mask, scandefintions_t def, int extra);
	bool pataCompare(BYTE* data, BYTE* sign, char* mask);
	DWORD findSignature(DWORD base, DWORD size, BYTE* sign, char* mask);
	
};

#endif
