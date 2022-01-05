#pragma once

#include <math.h>

#include "ProcessManager.h"
#include "IGameManager.h"

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


class TrackingSystem
{
public:
	TrackingSystem(ProcessManager* procManager, IGameManager* gameManager);
	~TrackingSystem();

	void pause();
	void resume();
	bool isPaused() const { return paused_; };

	void setTargetBase(DWORD newTargetBase);
	void trackingMainLoop();

	Coordinate_t getTargetCoordinate();
	LocalShit_t getLocalCoordinate();

private:
	ProcessManager* procManager_ = 0;
	IGameManager* gameManager_ = 0;
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

	HANDLE thread_ = 0;
	HANDLE mutex_ = 0;
	bool paused_ = true;
	bool terminateThread_ = false;

	static DWORD WINAPI threadInitiator(LPVOID vParams);

	void openTrackingThread();
	void closeTrackingThread();

	void adjustAim();
	void injectAngle(float pitch, float yaw);

	void printVerbose();
};
