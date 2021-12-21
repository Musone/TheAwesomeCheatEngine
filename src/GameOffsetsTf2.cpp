#include "GameOffsetsTf2.h"


void GameOffsetsTf2::init()
{
	procManager_ = new ProcessManager();
	dwClient_ = 0;
	dwClientSize_ = 0;
	dwEngine_ = 0;
	dwEngineSize_ = 0;
	dwLocalPlayer_ = 0;
	dwEntityList_ = 0;
	dwButtonBase_ = 0;
	dwButtonBase2_ = 0;
	dwDuck_ = 0;
	dwMoveForwards_ = 0;
	dwMoveBackwards_ = 0;
	dwJump_ = 0;
	dwAttack_ = 0;
	dwAttack2_ = 0;
	dwMoveLeft_ = 0;
	dwMoveRight_ = 0;
	dwLeft_ = 0;
	dwRight_ = 0;
	dwGetMaxClients_ = 0;
	dwIsInGame_ = 0;
	dwIsConnected_ = 0;
	dwGlowObjectManager_ = 0;
	dwWorldToScreen_ = 0;
	dwViewAngles_ = 0;
}

void GameOffsetsTf2::loadProcess()
{
	procManager_->Process("hl2.exe");

	while (!dwClient_ && !dwEngine_)
	{
		// why tf did the guy iterate over the process list again for the
		// module size?
		try
		{
			MODULEENTRY32 client_dll = procManager_->Module("client.dll");
			MODULEENTRY32 engine_dll = procManager_->Module("engine.dll");

			dwClient_ = (DWORD)client_dll.modBaseAddr; // Get the module
			dwClientSize_ = client_dll.modBaseSize;

			dwEngine_ = (DWORD)engine_dll.modBaseAddr;
			dwEngineSize_ = engine_dll.modBaseSize;
		}
		catch (const char* err) // scuffed error handling... we'll figure this out later
		{
			cout << err << endl;
			Sleep(1000);
		}
	}
}

GameOffsetsTf2::GameOffsetsTf2()
{
	// todo: init the process manager
	init();
	loadProcess();
	loadOffsets();
}

GameOffsetsTf2::~GameOffsetsTf2() { delete[] procManager_; }


void GameOffsetsTf2::save(const char* destFile)
{
	std::ofstream saveF;

	saveF.open(destFile); // Defines the file

	time_t theTime = time(NULL);
	struct tm* aTime = localtime(&theTime);

	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1;
	int year = aTime->tm_year + 1900;
	int hour = aTime->tm_hour;
	int minute = aTime->tm_min;
	int second = aTime->tm_sec;

	// Formatting things

	saveF << "[TF2 Offset Dump | ";
	saveF << day;
	saveF << "/";
	saveF << month;
	saveF << "/";
	saveF << year;
	saveF << " ";
	saveF << hour;
	saveF << ":";
	saveF << minute;
	saveF << ":";
	saveF << second; // Not the ideal method but eh
	saveF << "] \n\n";

	// Formatting the offsets

	saveF << "dwLocalPlayer : 0x" << hex << uppercase << dwLocalPlayer_
		<< "\n";
	saveF << "dwEntityList : 0x" << hex << uppercase << dwEntityList_
		<< "\n\n";

	saveF << "dwDuck : 0x" << hex << uppercase << dwDuck_ << "\n";
	saveF << "dwLeft : 0x" << hex << uppercase << dwLeft_ << "\n";
	saveF << "dwRight : 0x" << hex << uppercase << dwRight_ << "\n\n";
	saveF << "dwMoveLeft : 0x" << hex << uppercase << dwMoveLeft_ << "\n";
	saveF << "dwMoveRight : 0x" << hex << uppercase << dwMoveRight_ << "\n";
	saveF << "dwMoveForward : 0x" << hex << uppercase << dwMoveForwards_
		<< "\n";
	saveF << "dwMoveBackwards : 0x" << hex << uppercase << dwMoveBackwards_
		<< "\n\n";

	saveF << "dwJump : 0x" << hex << uppercase << dwJump_ << "\n";
	saveF << "dwAttack : 0x" << hex << uppercase << dwAttack_ << "\n";
	saveF << "dwAttack2 : 0x" << hex << uppercase << dwAttack2_ << "\n\n";

	saveF << "dwGetMaxClients : 0x" << hex << uppercase << dwGetMaxClients_
		<< "\n";
	saveF << "dwIsInGame : 0x" << hex << uppercase << dwIsInGame_ << "\n";
	saveF << "dwIsConnected : 0x" << hex << uppercase << dwIsConnected_
		<< "\n";
	saveF << "dwGlowObjectManager : 0x" << hex << uppercase
		<< dwGlowObjectManager_ << "\n";

	saveF << "dwViewAngles : 0x" << hex << uppercase << dwViewAngles_
		<< "\n";

	// Closed the files connection

	saveF.close();
}


void GameOffsetsTf2::printOffsets()
{
	printOffset("dwLocalPlayer",
	            dwLocalPlayer_); // Printing the offset for formatting
	printOffset("dwEntityList",
	            dwEntityList_); // Printing the offset for formatting

	cout << endl;

	printOffset("dwDuck", dwDuck_); // Printing the offset for formatting
	printOffset("dwLeft", dwLeft_); // Printing the offset for formatting
	printOffset("dwRight", dwRight_); // Printing the offset for formatting

	cout << endl;

	printOffset("dwMoveLeft", dwMoveLeft_); // Printing the offset for formatting
	printOffset("dwMoveRight", dwMoveRight_); // Printing the offset for formatting
	printOffset("dwMoveForwards", dwMoveForwards_); // Printing the offset for formatting
	printOffset("dwMoveBackwards", dwMoveBackwards_); // Printing the offset for formatting
	printOffset("dwJump", dwJump_); // Printing the offset for formatting
	printOffset("dwAttack", dwAttack_); // Printing the offset for formatting
	printOffset("dwAttack2", dwAttack2_); // Printing the offset for formatting

	cout << endl;

	printOffset("dwGetMaxClients", dwGetMaxClients_); // Printing the offset for formatting
	printOffset("dwIsInGame", dwIsInGame_); // Printing the offset for formatting
	printOffset("dwIsInConnected", dwIsConnected_); // Printing the offset for formatting
	printOffset("dwGlowObjectManager", dwGlowObjectManager_); // Printing the offset for formatting

	cout << endl;

	printOffset("dwViewAngles",
	            dwViewAngles_); // Printing the offset for formatting

	cout << endl;
}

void GameOffsetsTf2::loadOffsets()
{
	dwButtonBase_ = procManager_->findAddress(dwClient_, dwClientSize_,
	                                          (PBYTE)"\x68\x00\x00\x00\x00\x8B\x40\x28\xFF\xD0\xA1",
	                                          "x????xxxxxx", read, 1);

	dwButtonBase2_ = procManager_->findAddress(
		dwClient_, dwClientSize_,
		(PBYTE)"\xA1\x00\x00\x00\x00\x5E\xA8\x01\x75\x17",
		"x????xxxxx", read, 1);

	// Pattern Scanning for Local Player then printing.


	// todo: integrate this magic trash into the type system. Create a mock
	// class for the real object?
	// todo: What does local player and 
	dwLocalPlayer_ = procManager_->findAddress(
		dwClient_, dwClientSize_,
		(PBYTE)"\xA1\x00\x00\x00\x00\x33\xC9\x83\xC4\x04", "x????xxxxx",
		read, 1); // Sig scanning for localplayer address.

	// todo: Whydoes the entity list have to be offset by 0x18
	dwEntityList_ = procManager_->findAddress(
		dwClient_, dwClientSize_,
		(PBYTE)"\xA1\x00\x00\x00\x00\xA8\x01\x75\x51\x83\xC8\x01",
            "x????xxxxxxx", scandefintions_t::read,
		1); // Sig scanning for entitylist address.
	dwEntityList_ += 0x18;


	dwDuck_ = dwButtonBase_ + 0x5C;
	dwLeft_ = dwButtonBase2_ + 0x24;
	dwRight_ = dwButtonBase2_ + 0x30;
	dwMoveLeft_ = dwButtonBase2_ + 0x78;
	dwMoveRight_ = dwButtonBase2_ + 0x84;
	dwMoveForwards_ = dwButtonBase_ + 0xFCC;
	dwMoveBackwards_ = dwButtonBase_ + 0xFC0;
	dwJump_ = dwButtonBase_ + 0x20;
	dwAttack_ = dwButtonBase_ + 0x2C;
	dwAttack2_ = dwButtonBase_ + 0x38;

	// todo: misc?
	dwGetMaxClients_ =
		(DWORD)(procManager_->findAddress(dwEngine_, dwEngineSize_,
		                                  (PBYTE)"\x83\x3D\x00\x00\x00\x00\x00\x75\x38", "xx?????xx",
            scandefintions_t::read, 2));

	dwIsInGame_ = (DWORD)(procManager_->findAddress(
		dwEngine_, dwEngineSize_,
		(PBYTE)"\x83\x3D\x00\x00\x00\x00\x00\x0F\x9D\xC0\xC3", "xx?????xxxx",
		read, 2));

	dwIsConnected_ = (DWORD)(procManager_->findAddress(
		dwEngine_, dwEngineSize_,
		(BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\xE8\x00\x00"
		"\x00\x00\x6A\x00",
            "xx?????xx????x????xx", scandefintions_t::read, 2));

	dwGlowObjectManager_ = (DWORD)(procManager_->findAddress(
		dwClient_, dwClientSize_,
		(PBYTE)"\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xB0\x01\x5D",
            "x????x????xxx", scandefintions_t::read, 1));


	// World to screen is here but I'm not sure if it is the correct offset so I'm
	// not enabling it. If you want it just uncomment
	// Offset.dwWorldToScreen = (DWORD)(procManager_->findAddress(Offset.dwEngine,
	// Offset.dwEngineSize, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x01\xFF\x60\x7C",
	// "xx????xxxxx", scandefintions_t::read, 2)); printOffset("dwWorldToScreen",
	// Offset.dwWorldToScreen); // Printing the offset for formatting
	// printOffset("dwWorldToScreen", Offset.dwWorldToScreen + 0x34);

	dwViewAngles_ = (DWORD)(procManager_->findAddress(dwEngine_, dwEngineSize_,
	                                                  (PBYTE)"\xD9\x1D\x00\x00\x00\x00\xD9\x46\x04",
	                                                  "xx????xxx", scandefintions_t::read, 2));
}

void GameOffsetsTf2::printOffset(const char* gname, DWORD offset)
{
	cout << gname << " : ";
	cout << "0x";
	cout << hex << uppercase << offset << endl;
}
