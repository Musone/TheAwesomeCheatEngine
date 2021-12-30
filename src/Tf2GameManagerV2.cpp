#include "Tf2GameManagerV2.h"

Tf2GameManagerV2::Tf2GameManagerV2(ProcessManager* proc)
{
	procManager_ = proc;
	loadProcessAndModules();

	getEntityListBasePtr();
	getLocalPlayerBase();

	pitchBase_ = (float*)(engineBase_ + PITCH_OFFSET);
	yawBase_ = (float*)(engineBase_ + YAW_OFFSET);
}

PlayerInfo_t Tf2GameManagerV2::getPlayerInfo(DWORD playerBase)
{
	return loadPlayerInfo(playerBase);
}

PlayerInfo_t Tf2GameManagerV2::getLocalPlayerInfo()
{
	return loadPlayerInfo(localPlayerBase_);
}

ClientInfo_t Tf2GameManagerV2::getClientAtIndex(DWORD index) {
	ClientInfo_t client;
	procManager_->readAddress(entityListBase_ + sizeof(ClientInfo_t) * index,
	                          (BYTE*)&client, sizeof(client));
	return client;
}

PlayerInfo_t Tf2GameManagerV2::loadPlayerInfo(DWORD entityBase)
{
	// todo: requires that entityBase is not a fucking null pointer.
	// const DWORD clientInfoSize = 0x10;
	// const DWORD hpOffset = 0xA8;
	// const DWORD xOffset = 0x28c;
	// const DWORD yOffset = 0x290;
	// const DWORD zOffset = 0x294;
	// const DWORD observermodeOffset = 0x109C;
	// const DWORD cursoridOffset = 0x177C;
	//
	// DWORD x, y, z;

	if (!entityBase)
		throw "(Tf2GameManagerV2::loadPlayerInfo) Null pointer exception";

	PlayerInfo_t playerInfo = {0};
	Entity_t player;

	procManager_->readAddress(entityBase, (BYTE*)&player, sizeof(player));

	// todo: verbose shit ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	playerInfo.hp = player.hp;
	playerInfo.x = player.x;
	playerInfo.y = player.y;
	playerInfo.z = player.z;
	playerInfo.observermode = player.observermode;
	playerInfo.cursorid = player.cursorid;
	// todo: verbose shit ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	return playerInfo;
}


void Tf2GameManagerV2::loadProcessAndModules()
{
	procManager_->process(L"hl2.exe");

	while (!clientBase_ && !engineBase_)
	{
		try
		{
			MODULEENTRY32 client_dll = procManager_->module(L"client.dll");
			MODULEENTRY32 engine_dll = procManager_->module(L"engine.dll");

			clientBase_ = (DWORD)client_dll.modBaseAddr;
			clientModSize_ = client_dll.modBaseSize;

			engineBase_ = (DWORD)engine_dll.modBaseAddr;
			engineModSize_ = engine_dll.modBaseSize;
		}
		catch (const char* err) // scuffed error handling... we'll figure this out later
		{
			std::cout << err << std::endl;
			Sleep(1000);
		}
	}
}

void Tf2GameManagerV2::getLocalPlayerBase()
{
	const BYTE sig[] = {0xA1, 0x00, 0x00, 0x00, 0x00, 0x33, 0xC9, 0x83, 0xC4, 0x04};
	const char mask[] = "x????xxxxx";


	DWORD sigBase = procManager_->findSignature(clientBase_, clientModSize_, (BYTE*)sig, (char*)mask);
	DWORD localPlayerDoublePtr = procManager_->read<DWORD>(sigBase + 1);
	localPlayerBase_ = procManager_->read<DWORD>(localPlayerDoublePtr);
	if (!localPlayerBase_)
		throw "(Tf2GameManagerV2::getLocalPlayerBase) Local player base was a null pointer";
}


void Tf2GameManagerV2::getEntityListBasePtr()
{
	BYTE sig[] = {
		0xA1, 0x00, 0x00, 0x00, 0x00, 0xA8,
		0x01, 0x75, 0x51, 0x83, 0xC8, 0x01
	};
	char mask[] = "x????xxxxxxx";
	DWORD signatureBase = procManager_->findSignature((DWORD)clientBase_,
	                                                  clientModSize_,
	                                                  sig,
	                                                  mask);

	if (!signatureBase) throw "could not find signature";

	entityListBase_ = procManager_->read<DWORD>(signatureBase + 1);
	// todo: why do we add magic number 0x18?
	entityListBase_ += 0x18;
}

void Tf2GameManagerV2::testPrintHealths()
{
	ClientInfo_t entityList[MAX_PLAYERS];
	Entity_t ent;
	procManager_->readAddress(entityListBase_, (BYTE*)&entityList, sizeof(entityList));

	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		if (entityList[i].entity)
		{
			procManager_->readAddress((DWORD)entityList[i].entity, (BYTE*)&ent, sizeof(ent));
			printf("Player %d HP: %d\n", i, ent.hp);
		}
		else
			printf("No player at Entities[%d]\n", i);
	}
	std::cout << std::endl;
}


void Tf2GameManagerV2::printVerbose()
{
	// printf("%x\n\n", *entityListBase_);

	testPrintHealths();
	printf("entity list base: 0x%x\n", (DWORD)entityListBase_);
	printf("entity offset: <client.dll> + 0x%x\n\n", (DWORD)entityListBase_ - (DWORD)clientBase_);
	printf("client base: 0x%x\n", (DWORD)clientBase_);
	printf("engine base: 0x%x\n", (DWORD)engineBase_);
	printf("pitch base: 0x%x\n", (DWORD)pitchBase_);
	printf("yaw base: 0x%x\n", (DWORD)yawBase_);
	std::cout << std::endl;
}
