#pragma once

#include <windows.h>
// #include <winuser.h>
#include "Process.h"
// #include "Tf2GameManager.h"
#include "Tf2OffsetV2.h"
#include "Tf2Targetting.h"
#include "Structs.h"
#include <math.h>

using std::cout;
using std::endl;

class Tf2Aimbot
{
public:
	Tf2Aimbot();
	~Tf2Aimbot();

	void start();
	// todo: decide if i want to keep this
	// void switchToPrevTarget(); 
	void updateTarget();

	void pauseTracking();
	void resumeTracking();

	// void setTargetPlayerIndex(DWORD index) { targetPlayerIndex_ = index; };
	// DWORD targetPlayerIndex() { return targetPlayerIndex_; };

private:
	Process* procManager_ = 0;
	IOffset* gameManager_ = 0;
	Tf2Tracking* tracking_ = 0;
	Tf2Targetting* targetting_ = 0;

	// DWORD targetPlayerIndex_ = 1;
	// DWORD targetPlayerBase_ = 0;

	// static void nextIndex(Tf2Aimbot* obj);
	// static void prevIndex(Tf2Aimbot* obj);
	// void switchTarget(void (*getNewTarget)(Tf2Aimbot*));
	// bool isGoodTarget(ClientInfo_t client);

	void printPlayerInfo(PlayerInfo_t player);
};
