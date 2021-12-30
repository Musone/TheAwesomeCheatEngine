#pragma once

#define WIN32_LEAN_AND_MEAN //Excludes Headers We Wont Use (Increase Compile Time)

#include <Windows.h> //Standard Windows Functions/Data Types
#include <iostream> //Constains Input/Output Functions (cin/cout etc..)
#include <TlHelp32.h> //Contains read/Write Functions
#include <string> //Support For Strings
#include <sstream> //Supports Data Conversion

#pragma warning(disable : 4996)

class ProcessManager
{
	// todo: refactor the process manager
public:
	ProcessManager();
	~ProcessManager();

	//READ MEMORY
	template <class cData>
	cData read(DWORD dwAddress);
	template <class cData>
	cData readString(DWORD dwAddress);

	void process(LPCWSTR ProcessName); //Return Handle To The process
	MODULEENTRY32 module(LPCWSTR ModuleName); // Return module Base Address

	// todo: remove the unused garbage...
	DWORD findAddress(DWORD mod, DWORD modsize, BYTE* sig, const char* mask, int extra);
	void writeAddress(DWORD addr, BYTE* buff, SIZE_T nBytes);
	void readAddress(DWORD addr, BYTE* buff, SIZE_T nBytes);
	DWORD findSignature(DWORD base, DWORD size, BYTE* sig, char* mask);
private:
	// STORAGE
	HANDLE hProcess = 0;
	DWORD dwPID = 0, dwProtection = 0, dwCaveAddress = 0;

	// todo: this reads compares the our current position to the signature.
	// todo: add safety for if you try to read outside of the VAS of the process.
	bool sigEqual(BYTE* data, BYTE* sig, char* mask);

	// MISC
	BOOL bPOn = 0, bIOn = 0, bProt = 0;
};
