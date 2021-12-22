#ifndef GAMEOFFSET_H
#define GAMEOFFSET_H

#include <Windows.h>
#include <fstream>
#include <ctime>

#include "ProcessManager.h"

#define FOREGROUND_WHITE		    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW       	(FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN		        (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA	        (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK		    0

#define FOREGROUND_INTENSE_RED		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE		(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE	(FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW	(FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN		(FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA	(FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)

#pragma warning(disable : 4996)

using std::cout;
using std::hex;
using std::uppercase;
using std::endl;

class GameOffsetsTf2
{
 public:
	GameOffsetsTf2();
	~GameOffsetsTf2();

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
	///////////////////////////////////////////////////////////

	void save(const char*);
	void printOffsets();


private:
	ProcessManager* procManager_;

	// DLL Modules
	DWORD dwClient_;
	DWORD dwClientSize_;

	DWORD dwEngine_;
	DWORD dwEngineSize_;

	// Offsets
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

	void init();
	void loadProcess();
	void loadOffsets();
	void printOffset(const char* gname, DWORD offset);
};
#endif  // GAMEOFFSET_H
