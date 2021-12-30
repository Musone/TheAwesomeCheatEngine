#pragma once

#include "ProcessManager.h"
#include "Tf2GameManager.h"
#include "Tf2GameManagerV2.h"
#include <math.h>

#define PI 3.14159265


class Tf2Aimbot
{
public:
	Tf2Aimbot();
	~Tf2Aimbot();

	void trackPlayer();
	void start();

	void openTargettingThread(); // todo: make private

	void initThread();

	void switchToPrevTarget();
	void switchToNextTarget();
	ClientInfo_t getClientAtIndex(DWORD index);

private:
	ProcessManager* procManager_ = 0;
	IGameManager* gameManager_ = 0;
	DWORD localPlayerIndex_ = 0;
	DWORD targetPlayerIndex_ = 1;
	DWORD targetPlayerBase_ = 0;

	HANDLE thread_ = 0;
	HANDLE mutex_ = 0;
	bool terminateThread_ = false; // todo: find out if this needs to be atomic
	bool paused_ = true;

	DWORD threadId_ = 0;


	// DWORD WINAPI trackPlayerThread();
	void injectAngle(float pitch, float yaw);
	void printPlayerInfo(PlayerInfo_t player);
	void closeTargettingThread();
	void pauseThread();
	void resumeThread();
};

class ThreadParam
{
public:
	ThreadParam(Tf2Aimbot* obj, DWORD localPlayerIndex, DWORD enemyPlayerIndex)
	{
		obj_ = obj;
		localPlayerIndex_ = localPlayerIndex;
		enemyPlayerIndex_ = enemyPlayerIndex;
	};

	~ThreadParam()
	{
	};

	Tf2Aimbot* obj_;
	DWORD localPlayerIndex_;
	DWORD enemyPlayerIndex_;
};
