#include "GameOffsetsTf2.h"

void GameOffsetsTf2::init(ProcessManager* proc)
{
	procManager_ = proc;
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
	dwPitch_ = 0;
	dwYaw_ = 0;
}

GameOffsetsTf2::GameOffsetsTf2(ProcessManager* proc)
{
	// todo: init the process manager in Aimbot class instead of here.
	init(proc);
	loadProcess();
	getEntityListBasePtr();
	getViewAngles();

	// todo: figure out if I need the rest of the offsets that are setup in this shit
	// loadOffsets();
}

GameOffsetsTf2::~GameOffsetsTf2()
{
	// todo: don't delete here because technically Tf2Aimbot owns the process.
}

void GameOffsetsTf2::getViewAngles()
{
	dwPitch_ = dwEngine_ + PITCH_OFFSET;
	dwYaw_ = dwEngine_ + YAW_OFFSET;
}

PlayerInfo_t GameOffsetsTf2::getPlayerInfo(DWORD index)
{
	const DWORD hpOffset = 0xA8;
	const DWORD xOffset = 0x28c;
	const DWORD yOffset = 0x290;
	const DWORD zOffset = 0x294;
	const DWORD clientInfoSize = 0x10;

	DWORD x, y, z;
	PlayerInfo_t playerInfo;

	DWORD entityListIndex = dwEntityList_ + index * clientInfoSize;
	DWORD entityBase = procManager_->read<DWORD>(entityListIndex);

	x = procManager_->read<DWORD>(entityBase + xOffset);
	y = procManager_->read<DWORD>(entityBase + yOffset);
	z = procManager_->read<DWORD>(entityBase + zOffset);

	playerInfo.hp = procManager_->read<DWORD>(entityBase + hpOffset);
	playerInfo.x = *(float*)&x;
	playerInfo.y = *(float*)&y;
	playerInfo.z = *(float*)&z;

	return playerInfo;
}


void GameOffsetsTf2::getEntityListBasePtr()
{
	BYTE sig[] = {
		0xA1, 0x00, 0x00, 0x00, 0x00, 0xA8,
		0x01, 0x75, 0x51, 0x83, 0xC8, 0x01
	};
	char mask[] = "x????xxxxxxx";
	DWORD signatureBase = procManager_->findSignature(dwClient_,
	                                                  dwClientSize_,
	                                                  sig,
	                                                  mask); // Sig scanning for entitylist address.

	if (!signatureBase)
		throw "could not find signature";


	dwEntityList_ = procManager_->read<DWORD>(signatureBase + 1);
	// todo: why do we add magic number 0x18?
	dwEntityList_ += 0x18;

	// todo: subtract if you want the offset
	// dwEntityList_ -= dwClient_;
}

void GameOffsetsTf2::loadProcess()
{
	procManager_->process(L"hl2.exe");

	while (!dwClient_ && !dwEngine_)
	{
		try
		{
			MODULEENTRY32 client_dll = procManager_->module(L"client.dll");
			MODULEENTRY32 engine_dll = procManager_->module(L"engine.dll");

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


////// less important //////////////////////////////////////////////////////////////////////////////////////////////
// todo: Fix the crap below ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// todo: testing my own signature scanner for the function that controls ammo reload.
DWORD GameOffsetsTf2::testGetAmmo()
{
	BYTE sig[] = {
		0x55, 0x8B, 0xEC, 0x56, 0x57, 0x8B, 0x7D, 0x08, 0x8B, 0xF1, 0x85, 0xFF, 0x7E, 0x4F, 0x53, 0x8B,
		0x5D, 0x0C, 0x53, 0xE8, 0x58, 0x33, 0x29, 0x00, 0x8B, 0xC8, 0xE8, 0x41, 0xCB, 0xFE, 0xFF, 0x83,
		0xF8, 0xFE, 0x74, 0x38, 0x8B, 0x84, 0x9E, 0xC4, 0x06, 0x00, 0x00, 0x33, 0xC9, 0x2B, 0xC7, 0x8D,
		0xBB, 0xB1, 0x01, 0x00, 0x00, 0x85, 0xC0, 0x8D, 0x3C, 0xBE, 0x0F, 0x4F, 0xC8, 0x8B, 0x07, 0x89,
		0x4D, 0x08, 0x89, 0x4D, 0x0C, 0x8D, 0x4D, 0x08, 0x3B, 0x01, 0x74, 0x10, 0x8B, 0x06, 0x8B, 0xCE,
		0x57, 0xFF, 0x90, 0x1C, 0x05, 0x00, 0x00, 0x8B, 0x45, 0x0C, 0x89, 0x07, 0x5B, 0x5F, 0x5E, 0x5D,
		0xC2, 0x08, 0x00
	};
	BYTE toWrite[] = {0x90, 0x90};
	char bitMask[] =
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

	DWORD overwriteAddr;
	DWORD functionVA;
	const DWORD offsetIntoFunction = 90;
	MODULEENTRY32 server_dll;

	try
	{
		server_dll = procManager_->module(L"server.dll");
		functionVA = procManager_->findAddress((DWORD)server_dll.modBaseAddr,
		                                       (DWORD)server_dll.modBaseSize,
		                                       sig, (char*)bitMask, 0);

		overwriteAddr = functionVA + offsetIntoFunction;
		procManager_->writeAddress(overwriteAddr, toWrite, 2);

		printOffset("TEST GET AMMO: ", overwriteAddr);
		return overwriteAddr;
	}
	catch (const char* err)
	{
		cout << err << endl;
		Sleep(5000);
		exit(0);
		return 0;
	}
}


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
	printOffset("dwLocalPlayer", dwLocalPlayer_); // Printing the offset for formatting
	printOffset("dwEntityList", dwEntityList_); // Printing the offset for formatting

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
	                                          "x????xxxxxx", 1);

	dwButtonBase2_ = procManager_->findAddress(dwClient_, dwClientSize_,
	                                           (PBYTE)"\xA1\x00\x00\x00\x00\x5E\xA8\x01\x75\x17",
	                                           "x????xxxxx", 1);

	// Pattern Scanning for Local Player then printing.


	// todo: integrate this magic trash into the type system. Create a mock
	// class for the real object?
	// todo: What does local player and 
	dwLocalPlayer_ = procManager_->findAddress(
		dwClient_, dwClientSize_,
		(PBYTE)"\xA1\x00\x00\x00\x00\x33\xC9\x83\xC4\x04", "x????xxxxx",
		1); // Sig scanning for localplayer address.

	// todo: Whydoes the entity list have to be offset by 0x18
	dwEntityList_ = procManager_->findAddress(dwClient_, dwClientSize_,
	                                          (PBYTE)"\xA1\x00\x00\x00\x00\xA8\x01\x75\x51\x83\xC8\x01",
	                                          "x????xxxxxxx",
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
		                                  2));

	dwIsInGame_ = (DWORD)(procManager_->findAddress(
		dwEngine_, dwEngineSize_,
		(PBYTE)"\x83\x3D\x00\x00\x00\x00\x00\x0F\x9D\xC0\xC3", "xx?????xxxx",
		2));

	dwIsConnected_ = (DWORD)(procManager_->findAddress(
		dwEngine_, dwEngineSize_,
		(BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\xE8\x00\x00"
		"\x00\x00\x6A\x00",
		"xx?????xx????x????xx", 2));

	dwGlowObjectManager_ = (DWORD)(procManager_->findAddress(
		dwClient_, dwClientSize_,
		(PBYTE)"\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xB0\x01\x5D",
		"x????x????xxx", 1));


	// World to screen is here but I'm not sure if it is the correct offset so I'm
	// not enabling it. If you want it just uncomment
	// Offset.dwWorldToScreen = (DWORD)(procManager_->findAddress(Offset.dwEngine,
	// Offset.dwEngineSize, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x01\xFF\x60\x7C",
	// "xx????xxxxx", scandefintions_t::read, 2)); printOffset("dwWorldToScreen",
	// Offset.dwWorldToScreen); // Printing the offset for formatting
	// printOffset("dwWorldToScreen", Offset.dwWorldToScreen + 0x34);

	dwViewAngles_ = (DWORD)(procManager_->findAddress(dwEngine_, dwEngineSize_,
	                                                  (PBYTE)"\xD9\x1D\x00\x00\x00\x00\xD9\x46\x04",
	                                                  "xx????xxx", 2));
}

void GameOffsetsTf2::printOffset(const char* gname, DWORD offset)
{
	cout << gname << " : ";
	cout << "0x";
	cout << hex << uppercase << offset << endl;
}
