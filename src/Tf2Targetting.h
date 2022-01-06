#pragma once

#include "Process.h"
#include "Structs.h"
#include "Tf2Tracking.h"

#define PI 3.14159265
#define MAX_PLAYERS 24

class Tf2Targetting
{
public:
	Tf2Targetting(Process* process, Tf2Tracking* tracking, DWORD localPlayerBase, DWORD entListBase);
	~Tf2Targetting();

	DWORD bestTargetBase() const { return bestTargetBase_; }


	void pause();
	void resume();

	// external switch functions
	void extSwitchToNextTarget();
	void extSwitchToPrevTarget();

protected:
	void mainLoop();
	DWORD targetPlayerIndex() const { return targetPlayerIndex_; };
	void setTargetPlayerIndex(DWORD newIndex) { targetPlayerIndex_ = newIndex; }

private:
	Process* process_ = 0;
	Tf2Tracking* tracking_ = 0;

	bool paused_ = false;
	DWORD entityListBase_ = 0;
	bool terminateThread_ = false;
	HANDLE thread_ = 0;
	HANDLE mutex_ = 0;
	DWORD localPlayerBase_ = 0;

	DWORD bestTargetBase_ = 0;
	DWORD targetPlayerIndex_ = 0;

	static DWORD WINAPI threadInit(LPVOID vParams);
	void openThread();
	void closeThread();

	bool isGoodTarget(DWORD targetBase);

	void switchToNextTarget();
	void switchToPrevTarget();

	void switchTarget(void (*getNewTarget)(Tf2Targetting*));

	ClientInfo_t getClientAtIndex(DWORD index);

	static void nextIndex(Tf2Targetting* obj);
	static void prevIndex(Tf2Targetting* obj);
};
