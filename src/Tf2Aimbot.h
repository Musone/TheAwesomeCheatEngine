#pragma once

#include <windows.h>
// #include <winuser.h>
#include "ProcessManager.h"
#include "Tf2GameManager.h"
#include "Tf2GameManagerV2.h"
#include "TrackingSystem.h"
#include <math.h>

#define PI 3.14159265


class Tf2Aimbot
{
public:
	Tf2Aimbot();
	~Tf2Aimbot();

	void start();
	void switchToPrevTarget();
	void switchToNextTarget();

	void pauseTracking();
	void resumeTracking();

	void setTargetPlayerIndex(DWORD index) { targetPlayerIndex_ = index; };
	DWORD targetPlayerIndex() { return targetPlayerIndex_; };

private:
	ProcessManager* procManager_ = 0;
	IGameManager* gameManager_ = 0;
	TrackingSystem* trackingSystem_ = 0;

	DWORD targetPlayerIndex_ = 1;
	DWORD targetPlayerBase_ = 0;

	static void nextIndex(Tf2Aimbot* obj);
	static void prevIndex(Tf2Aimbot* obj);
	void switchTarget(void (*getNewTarget)(Tf2Aimbot*));
	bool isGoodTarget(ClientInfo_t client);

	void printPlayerInfo(PlayerInfo_t player);
};
