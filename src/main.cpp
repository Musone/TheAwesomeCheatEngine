#include "GameOffsetsTf2.h"
#include "ProcessManager.h"
// #include "CNetVar.h"
// #include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <ctime>

#pragma warning(disable : 4996)

using std::cout;
using std::endl;
using std::uppercase;
using std::hex;

ProcessManager Mem;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
bool close = false;

void PrintOffset(const char* gname, DWORD offset)
{
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSE_WHITE);
	cout << gname << " : ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "0x";
	cout << hex << uppercase << offset << endl;
}

// void Save()
// {
// 	std::ofstream saveF;
//
// 	saveF.open("tf2_offsets.txt"); // Defines the file
//
// 	time_t theTime = time(NULL);
// 	struct tm* aTime = localtime(&theTime);
//
// 	int day = aTime->tm_mday;
// 	int month = aTime->tm_mon + 1;
// 	int year = aTime->tm_year + 1900;
// 	int hour = aTime->tm_hour;
// 	int minute = aTime->tm_min;
// 	int second = aTime->tm_sec;
//
//
// 	//Formatting things
//
// 	saveF << "[TF2 Offset Dump | ";
// 	saveF << day;
// 	saveF << "/";
// 	saveF << month;
// 	saveF << "/";
// 	saveF << year;
// 	saveF << " ";
// 	saveF << hour;
// 	saveF << ":";
// 	saveF << minute;
// 	saveF << ":";
// 	saveF << second; // Not the ideal method but eh
// 	saveF << "] \n\n";
//
// 	//Formatting the offsets
//
// 	saveF << "dwLocalPlayer : 0x" << hex << uppercase << Offset.dwLocalPlayer << "\n";
// 	saveF << "dwEntityList : 0x" << hex << uppercase << Offset.dwEntityList << "\n\n";
//
// 	saveF << "dwDuck : 0x" << hex << uppercase << Offset.dwDuck << "\n";
// 	saveF << "dwLeft : 0x" << hex << uppercase << Offset.dwLeft << "\n";
// 	saveF << "dwRight : 0x" << hex << uppercase << Offset.dwRight << "\n\n";
// 	saveF << "dwMoveLeft : 0x" << hex << uppercase << Offset.dwMoveLeft << "\n";
// 	saveF << "dwMoveRight : 0x" << hex << uppercase << Offset.dwMoveRight << "\n";
// 	saveF << "dwMoveForward : 0x" << hex << uppercase << Offset.dwMoveForwards << "\n";
// 	saveF << "dwMoveBackwards : 0x" << hex << uppercase << Offset.dwMoveBackwards << "\n\n";
//
// 	saveF << "dwJump : 0x" << hex << uppercase << Offset.dwJump << "\n";
// 	saveF << "dwAttack : 0x" << hex << uppercase << Offset.dwAttack << "\n";
// 	saveF << "dwAttack2 : 0x" << hex << uppercase << Offset.dwAttack2 << "\n\n";
//
// 	saveF << "dwGetMaxClients : 0x" << hex << uppercase << Offset.dwGetMaxClients << "\n";
// 	saveF << "dwIsInGame : 0x" << hex << uppercase << Offset.dwIsInGame << "\n";
// 	saveF << "dwIsConnected : 0x" << hex << uppercase << Offset.dwIsConnected << "\n";
// 	saveF << "dwGlowObjectManager : 0x" << hex << uppercase << Offset.dwGlowObjectManager << "\n";
//
// 	saveF << "dwViewAngles : 0x" << hex << uppercase << Offset.dwViewAngles << "\n";
//
// 	//Closed the files connection
//
// 	saveF.close();
// }

int main()
{
	//Resizing and formatting the window, it's important.
	SetConsoleTitle(L"TF2 Offset Dumper by Your mom, bech");
	HWND window = GetConsoleWindow();
	MoveWindow(window, 100, 100, 480, 480, true);


	// POLL GAME WINDOW ////////////////////////////////////////////
	if (!FindWindow(NULL, L"Team Fortress 2"))
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "Please Open TF2" << endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSE_WHITE);
	}

	while (!FindWindow(NULL, L"Team Fortress 2")) { Sleep(500); }
	system("cls"); //Clears the console
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "TF2 has been found." << endl << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSE_WHITE);
	////////////////////////////////////////////////////////////////


	//Hooking the main game innit
	Mem.Process("hl2.exe");

	//Getting module addresses
	while (!Offset.dwClient && !Offset.dwEngine)
	{
		// why tf did the guy iterate over the process list again for the module size?
		try
		{
			MODULEENTRY32 client_dll = Mem.Module("client.dll");
			MODULEENTRY32 engine_dll = Mem.Module("client.dll");

			Offset.dwClient = (DWORD)client_dll.modBaseAddr; // Get the module
			Offset.dwClientSize = client_dll.modBaseSize;

			Offset.dwEngine = (DWORD)engine_dll.modBaseAddr;
			Offset.dwEngineSize = engine_dll.modBaseSize;
		}
		catch (const char* err) // scuffed error handling... we'll figure this out later
		{
			cout << err << endl;
			Sleep(1000);
		}
	}

	Offset.dwButtonBase = Mem.FindAddress(Offset.dwClient, Offset.dwClientSize,
	                                      (PBYTE)"\x68\x00\x00\x00\x00\x8B\x40\x28\xFF\xD0\xA1", "x????xxxxxx",
	                                      scandefintions_t::read, 1);
	Offset.dwButtonBase_2 = Mem.FindAddress(Offset.dwClient, Offset.dwClientSize,
	                                        (PBYTE)"\xA1\x00\x00\x00\x00\x5E\xA8\x01\x75\x17", "x????xxxxx",
	                                        scandefintions_t::read, 1);


	//Pattern Scanning for Local Player then printing. 
	Offset.dwLocalPlayer = Mem.FindAddress(Offset.dwClient, Offset.dwClientSize,
	                                       (PBYTE)"\xA1\x00\x00\x00\x00\x33\xC9\x83\xC4\x04", "x????xxxxx",
	                                       scandefintions_t::read, 1); //Sig scanning for localplayer address.
	PrintOffset("dwLocalPlayer", Offset.dwLocalPlayer); // Printing the offset for formatting

	Offset.dwEntityList = Mem.FindAddress(Offset.dwClient, Offset.dwClientSize,
	                                      (PBYTE)"\xA1\x00\x00\x00\x00\xA8\x01\x75\x51\x83\xC8\x01", "x????xxxxxxx",
	                                      scandefintions_t::read, 1); //Sig scanning for entitylist address.
	Offset.dwEntityList += 0x18;
	PrintOffset("dwEntityList", Offset.dwEntityList); // Printing the offset for formatting

	cout << endl;

	Offset.dwDuck = Offset.dwButtonBase + 0x5C;
	PrintOffset("dwDuck", Offset.dwDuck); // Printing the offset for formatting

	Offset.dwLeft = Offset.dwButtonBase_2 + 0x24;
	PrintOffset("dwLeft", Offset.dwLeft); // Printing the offset for formatting

	Offset.dwRight = Offset.dwButtonBase_2 + 0x30;
	PrintOffset("dwRight", Offset.dwRight); // Printing the offset for formatting

	cout << endl;

	Offset.dwMoveLeft = Offset.dwButtonBase_2 + 0x78;
	PrintOffset("dwMoveLeft", Offset.dwMoveLeft); // Printing the offset for formatting

	Offset.dwMoveRight = Offset.dwButtonBase_2 + 0x84;
	PrintOffset("dwMoveRight", Offset.dwMoveRight); // Printing the offset for formatting

	Offset.dwMoveForwards = Offset.dwButtonBase + 0xFCC;
	PrintOffset("dwMoveForwards", Offset.dwMoveForwards); // Printing the offset for formatting

	Offset.dwMoveBackwards = Offset.dwButtonBase + 0xFC0;
	PrintOffset("dwMoveBackwards", Offset.dwMoveBackwards); // Printing the offset for formatting

	cout << endl;

	Offset.dwJump = Offset.dwButtonBase + 0x20;
	PrintOffset("dwJump", Offset.dwJump); // Printing the offset for formatting

	Offset.dwAttack = Offset.dwButtonBase + 0x2C;
	PrintOffset("dwAttack", Offset.dwAttack); // Printing the offset for formatting

	Offset.dwAttack2 = Offset.dwButtonBase + 0x38;
	PrintOffset("dwAttack2", Offset.dwAttack2); // Printing the offset for formatting

	cout << endl;

	Offset.dwGetMaxClients = (DWORD)(Mem.FindAddress(Offset.dwEngine, Offset.dwEngineSize,
	                                                 (PBYTE)"\x83\x3D\x00\x00\x00\x00\x00\x75\x38", "xx?????xx",
	                                                 scandefintions_t::read, 2));
	PrintOffset("dwGetMaxClients", Offset.dwGetMaxClients); // Printing the offset for formatting

	Offset.dwIsInGame = (DWORD)(Mem.FindAddress(Offset.dwEngine, Offset.dwEngineSize,
	                                            (PBYTE)"\x83\x3D\x00\x00\x00\x00\x00\x0F\x9D\xC0\xC3", "xx?????xxxx",
	                                            scandefintions_t::read, 2));
	PrintOffset("dwIsInGame", Offset.dwIsInGame); // Printing the offset for formatting

	Offset.dwIsConnected = (DWORD)(Mem.FindAddress(Offset.dwEngine, Offset.dwEngineSize,
	                                               (BYTE*)
	                                               "\x80\x3D\x00\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x00",
	                                               "xx?????xx????x????xx", scandefintions_t::read, 2));
	PrintOffset("dwIsInConnected", Offset.dwIsConnected); // Printing the offset for formatting

	Offset.dwGlowObjectManager = (DWORD)(Mem.FindAddress(Offset.dwClient, Offset.dwClientSize,
	                                                     (PBYTE)"\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xB0\x01\x5D",
	                                                     "x????x????xxx", scandefintions_t::read, 1));
	PrintOffset("dwGlowObjectManager", Offset.dwGlowObjectManager); // Printing the offset for formatting

	cout << endl;

	// World to screen is here but I'm not sure if it is the correct offset so I'm not enabling it. If you want it just uncomment
	//Offset.dwWorldToScreen = (DWORD)(Mem.FindAddress(Offset.dwEngine, Offset.dwEngineSize, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x01\xFF\x60\x7C", "xx????xxxxx", scandefintions_t::read, 2));
	//PrintOffset("dwWorldToScreen", Offset.dwWorldToScreen); // Printing the offset for formatting
	//PrintOffset("dwWorldToScreen", Offset.dwWorldToScreen + 0x34);

	Offset.dwViewAngles = (DWORD)(Mem.FindAddress(Offset.dwEngine, Offset.dwEngineSize,
	                                              (PBYTE)"\xD9\x1D\x00\x00\x00\x00\xD9\x46\x04", "xx????xxx",
	                                              scandefintions_t::read, 2));
	PrintOffset("dwViewAngles", Offset.dwViewAngles); // Printing the offset for formatting

	cout << endl;

	//char yn;

	//SetConsoleTextAttribute(hConsole, FOREGROUND_CYAN);

	//cout << "Do you want to dump the netvars to a file [Y] Yes | [N] No : ";
	//std::cin >> yn;

	//if (yn == 'Y')
	//	DumpNetVars();

	//cout << endl;

	char yn;

	SetConsoleTextAttribute(hConsole, FOREGROUND_CYAN);

	cout << "Do you want to save to a file [Y] Yes | [N] No : ";
	std::cin >> yn;

	if (yn == 'Y' || yn == 'y')
		Save();

	cout << "Bye!";
	Sleep(300);
}
