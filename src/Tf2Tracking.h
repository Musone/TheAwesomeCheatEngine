#pragma once

#include <math.h>

#include "Process.h"
#include "IOffset.h"

#define PI 3.14159265


typedef struct Coordinate
{
	float x;
	float y;
	float z;
} Coordinate_t;


typedef struct LocalShit
{
	float pitch;
	float yaw;
	float roll;
	BYTE pad[96];
	float x;
	float y;
	float z;
} LocalShit_t;


class Tf2Tracking
{
public:
	Tf2Tracking(Process* procManager, IOffset* gameManager);
	~Tf2Tracking();

	void pause();
	void resume();
	bool isPaused() const { return paused_; };

	void setTargetBase(DWORD newTargetBase);

	void toggleRageMode();

	Coordinate_t getTargetCoordinate();
	LocalShit_t getLocalCoordinate();

protected:
	void mainLoop();

private:
	Process* proc_ = 0;
	IOffset* gameManager_ = 0;
	DWORD localPlayerBase_ = 0;
	DWORD targetPlayerBase_ = 0;
	DWORD targetBoneBase_ = 0;

	PlayerInfo_t localPlayer = {0};
	PlayerInfo_t targetPlayer = {0};
	float goalPitch_ = 0.0f;
	float goalYaw_ = 0.0f;
	float dx_ = 0.0f;
	float dy_ = 0.0f;
	float dr_ = 0.0f;
	float dz_ = 0.0f;
	float dYaw_ = 0.0f;
	float dPitch_ = 0.0f;
	float newPitch_ = 0.0f;
	float newYaw_ = 0.0f;
	bool rageMode_ = false;

	HANDLE thread_ = 0;
	HANDLE mutex_ = 0;
	bool paused_ = true;
	bool terminateThread_ = false;

	static DWORD WINAPI threadInit(LPVOID vParams);

	void openThread();
	void closeThread();

	void adjustAim();
	void injectAngle(float pitch, float yaw);

	void printVerbose();
};
