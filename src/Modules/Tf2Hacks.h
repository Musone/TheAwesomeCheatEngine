#pragma once
#include <math.h>

#include <windows.h>
#include <chrono>

#include "Process.h"
#include "Tf2OffsetV2.h"
#include "Tf2Targetting.h"
#include "../Resources/StructsAndResources.h"
#include "../Overlay/Tf2Overlay.h"


// todo: remove this ~~~~~~~~~~~~~
// using namespace std::chrono;
// using std::cout;
// using std::endl;
// todo: remove this ~~~~~~~~~~~~~


class Tf2Hacks
{
public:
	Tf2Hacks(HINSTANCE hInstance, int nCmdShow);
	~Tf2Hacks();

	void start();
	// todo: decide if i want to keep this
	// void switchToPrevTarget(); 
	// void updateTarget();
	//
	// void pauseTracking();
	// void resumeTracking();

	// void setTargetPlayerIndex(DWORD index) { targetPlayerIndex_ = index; };
	// DWORD targetPlayerIndex() { return targetPlayerIndex_; };

private:
	Process* process_ = 0;
	IOffset* offsets_ = 0;
	Tf2Tracking* tracking_ = 0;
	Tf2Targetting* targetting_ = 0;
	Local* local_ = 0;
	Screen* screen_;


	HANDLE espThread_ = 0;
	Tf2Overlay* overlay_ = 0;


	// DWORD targetPlayerIndex_ = 1;
	// DWORD targetPlayerBase_ = 0;

	// static void nextIndex(Tf2Hacks* obj);
	// static void prevIndex(Tf2Hacks* obj);
	// void switchTarget(void (*getNewTarget)(Tf2Hacks*));
	// bool isGoodTarget(ClientInfo_t client);

	void printPlayerInfo(PlayerInfo_t player);
};
