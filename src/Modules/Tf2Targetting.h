#pragma once

#include "Process.h"
#include "../Resources/StructsAndResources.h"
#include "../Resources/Geometry.h"
#include "Tf2Tracking.h"


class Tf2Targetting
{
public:
	Tf2Targetting(IOffset* offsets, Process* process, Tf2Tracking* tracking,
	              Local* local, Screen* screen, DWORD localPlayerBase,
	              DWORD entListBase);
	~Tf2Targetting();

	DWORD bestTargetBase() const { return bestTargetBase_; }


	void pause();
	void resume();
	bool isPaused() const { return paused_; };

	// external switch functions
	void extSwitchToNextTarget();
	void extSwitchToPrevTarget();

	// bool targetClosestPlayer();
	bool targetPlayerInFov();

protected:
	void mainLoop();
	DWORD targetPlayerIndex() const { return targetPlayerIndex_; };
	void setTargetPlayerIndex(DWORD newIndex) { targetPlayerIndex_ = newIndex; }

private:
	Process* process_ = 0;
	Tf2Tracking* tracking_ = 0;
	Local* local_ = 0;
	Screen* screen_ = 0;
	IOffset* offsets_ = 0;
	Geometry Geometry_;

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
	bool isInFov(Vec3_t pos);


	static void nextIndex(Tf2Targetting* obj);
	static void prevIndex(Tf2Targetting* obj);
};
