#pragma once

#include "ProcessManager.h"
#include "GameOffsetsTf2.h"
#include <math.h>

#define PI 3.14159265

class Tf2Aimbot
{
public:
	Tf2Aimbot();
	~Tf2Aimbot();

	void testTrackBot1();

private:
	ProcessManager* procManager_;
	GameOffsetsTf2* offsets_;

	void injectAngle(float pitch, float yaw);
	void printPlayerInfo(PlayerInfo_t player);
};
