#pragma once

#include <Windows.h>
#include <fstream>
#include <ctime>

#include "IOffset.h"
#include "Process.h"

#pragma warning(disable : 4996)

using std::cout;
using std::hex;
using std::uppercase;
using std::endl;

#define PITCH_OFFSET 0x467474
#define YAW_OFFSET PITCH_OFFSET + 4


class Tf2Offset : public IOffset
{
public:
	Tf2Offset(Process* proc);
	~Tf2Offset();

	// Getters /////////////////////////////////////////////
	DWORD dwClient() const { return dwClient_; }
	DWORD dwClientSize() const { return dwClientSize_; }
	DWORD dwEngine() const { return dwEngine_; }
	DWORD dwEngineSize() const { return dwEngineSize_; }
	DWORD dwLocalPlayer() const { return dwLocalPlayer_; }
	DWORD dwEntityList() const { return dwEntityList_; }
	DWORD dwButtonBase() const { return dwButtonBase_; }
	DWORD dwButtonBase2() const { return dwButtonBase2_; }
	DWORD dwDuck() const { return dwDuck_; }
	DWORD dwMoveForwards() const { return dwMoveForwards_; }
	DWORD dwMoveBackwards() const { return dwMoveBackwards_; }
	DWORD dwJump() const { return dwJump_; }
	DWORD dwAttack() const { return dwAttack_; }
	DWORD dwAttack2() const { return dwAttack2_; }
	DWORD dwMoveLeft() const { return dwMoveLeft_; }
	DWORD dwMoveRight() const { return dwMoveRight_; }
	DWORD dwLeft() const { return dwLeft_; }
	DWORD dwRight() const { return dwRight_; }
	DWORD dwGetMaxClients() const { return dwGetMaxClients_; }
	DWORD dwIsInGame() const { return dwIsInGame_; }
	DWORD dwIsConnected() const { return dwIsConnected_; }
	DWORD dwGlowObjectManager() const { return dwGlowObjectManager_; }
	DWORD dwWorldToScreen() const { return dwWorldToScreen_; }
	DWORD dwViewAngles() const { return dwViewAngles_; }
	DWORD dwPitchBase() const override { return dwPitch_; }
	DWORD dwYawBase() const override { return dwYaw_; }
	///////////////////////////////////////////////////////////

	void printVerbose() override;
	PlayerInfo_t getPlayerInfo(DWORD index) /*override*/;
	PlayerInfo_t getLocalPlayerInfo() override { throw "stub"; };
	ClientInfo_t getClientAtIndex(DWORD index) /*override*/ { throw "stub"; };

	void save(const char*);


	DWORD testGetAmmo();

private:
	Process* procManager_;

	// DLL Modules
	DWORD dwClient_;
	DWORD dwClientSize_;

	DWORD dwEngine_;
	DWORD dwEngineSize_;

	// Offsets
	DWORD ptrPlayerX_;
	DWORD ptrPlayerY_;
	DWORD ptrPlayerZ_;

	DWORD dwLocalPlayer_;
	DWORD dwEntityList_;

	DWORD dwButtonBase_;
	DWORD dwButtonBase2_;
	DWORD dwDuck_;
	DWORD dwMoveForwards_;
	DWORD dwMoveBackwards_;

	DWORD dwJump_;
	DWORD dwAttack_;
	DWORD dwAttack2_;

	DWORD dwMoveLeft_;
	DWORD dwMoveRight_;

	DWORD dwLeft_;
	DWORD dwRight_;

	DWORD dwGetMaxClients_;
	DWORD dwIsInGame_;
	DWORD dwIsConnected_;

	DWORD dwGlowObjectManager_;
	DWORD dwWorldToScreen_;

	DWORD dwViewAngles_;

	DWORD dwPitch_; // address of viewangle pitch
	DWORD dwYaw_; // address of viewangle yaw

	void init(Process* proc);
	void loadProcess();
	void loadOffsets();
	void printOffset(const char* gname, DWORD offset);
	void getEntityListBasePtr();
	void getViewAngles();
};
