#pragma once

#include <math.h>

#include "Process.h"
#include "IOffset.h"
#include "../Resources/StructsAndResources.h"
#include "../Resources/Geometry.h"
#include "../Resources/Local.h"

// #define PI 3.14159265

class Tf2Tracking
{
public:
	Tf2Tracking(Process* procManager, IOffset* offsets, Local* local, DWORD engineBase);
	~Tf2Tracking();

	void pause();
	void resume();
	bool isPaused() const { return paused_; };

	void setTargetBase(DWORD newTargetBase);

	void toggleRageMode();
	bool isRageMode() const { return rageMode_; };

	Vec3_t getTargetCoordinate();
	Vec3_t getPlayerCoordinate(DWORD playerBase);
	// LocalCoords_t getLocalCoordinate();

	// Vec3_t getAbsPlayerCoordinate(DWORD playerBase);
	// Angle_t getAbsoluteAngle(Vec3_t player, LocalCoords_t local);
	// Angle_t getRelativeAngle(Vec3_t player, LocalCoords_t local);

protected:
	void mainLoop();

private:
	const float smoothFactor_ = 3.0f;

	DWORD engineBase_ = 0;

	Process* process_ = 0;
	IOffset* gameManager_ = 0;
	Local* local_ = 0;
	DWORD localPlayerBase_ = 0;
	DWORD targetPlayerBase_ = 0;
	DWORD targetBoneBase_ = 0;

	PlayerInfo_t localPlayer = {0};
	PlayerInfo_t targetPlayer = {0};
	Geometry Geometry_;
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

	// void printVerbose();
};
