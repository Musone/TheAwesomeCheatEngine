#include "Tf2Tracking.h"

// todo: Find eye position, and use that for local player position in tracking system. (find view-matrix)
// todo: Check if player is on my team before choosing a target
// todo: Check if player is dead before choosing target todo:Add targetting FOV
// todo:
// todo: 

Tf2Tracking::Tf2Tracking(Process* procManager, IOffset* gameManager)
{
	proc_ = procManager;
	gameManager_ = gameManager;
	mutex_ = CreateMutexA(NULL, TRUE, NULL);
	openThread();
}

Tf2Tracking::~Tf2Tracking()
{
	closeThread();
	CloseHandle(thread_);
	CloseHandle(mutex_);
}


void Tf2Tracking::setTargetBase(DWORD newTargetBase)
{
	DWORD boneBaseOffset = 0x5B0;
	if (paused_)
	{
		targetPlayerBase_ = newTargetBase;
		targetBoneBase_ = proc_->read<DWORD>(targetPlayerBase_ + boneBaseOffset);
		// todo: This shouldn't be static...
	}
	else
	{
		pause();
		targetPlayerBase_ = newTargetBase;
		targetBoneBase_ = proc_->read<DWORD>(targetPlayerBase_ + boneBaseOffset);
		// todo: This shouldn't be static...
		resume();
	}
}

void Tf2Tracking::pause()
{
	if (paused_) return;
	WaitForSingleObject(mutex_, INFINITE);
	paused_ = true;
}

void Tf2Tracking::resume()
{
	if (!paused_) return;
	if (!ReleaseMutex(mutex_))
		throw "(Tf2Aimbot/resumeThread) Releasing mutex twice";
	paused_ = false;
}

void Tf2Tracking::mainLoop()
{
	while (1)
	{
		if (WAIT_FAILED == WaitForSingleObject(mutex_, INFINITE))
			throw "(tf2Aimbot/testThread) wait failed";

		if (terminateThread_)
		{
			std::cout << "TERMINATING!!!!\n";
			ReleaseMutex(mutex_);
			return;
		}
		adjustAim();
		ReleaseMutex(mutex_);
	}
}

DWORD WINAPI Tf2Tracking::threadInit(LPVOID vthis)
{
	Tf2Tracking* obj = (Tf2Tracking*)vthis;
	obj->mainLoop();
	return 0;
}

void Tf2Tracking::openThread()
{
	thread_ = CreateThread(NULL, 0, threadInit, (LPVOID)this, 0, NULL);
}

void Tf2Tracking::closeThread()
{
	pause();
	terminateThread_ = true;
	resume();
	WaitForSingleObject(thread_, INFINITE);
}

LocalShit_t Tf2Tracking::getLocalCoordinate()
{
	// todo: This shouldn't be static
	LocalShit_t result;
	proc_->readAddress(gameManager_->dwLocalPlayerBase() + 0x2f8, (BYTE*)&result, sizeof(result));
	return result;
}

Coordinate_t Tf2Tracking::getTargetCoordinate()
{
	// todo: maybe this shouldnt be handled here? hmm...
	Coordinate_t result;
	BoneMatrix_t bonem;

	proc_->readAddress(targetBoneBase_, (BYTE*)&bonem, sizeof(bonem));
	result.x = bonem.headx;
	result.y = bonem.heady;
	result.z = bonem.headz;
	return result;
}

void Tf2Tracking::toggleRageMode()
{
	if (paused_)
	{
		rageMode_ = !rageMode_;
	}
	else
	{
		pause();
		rageMode_ = !rageMode_;
		resume();
	}
}

void Tf2Tracking::adjustAim()
{
	// todo: FIX JITTERING
	float xySlope, rzSlope;
	Coordinate_t target;
	PlayerInfo_t local;

	target = getTargetCoordinate();
	local = gameManager_->getLocalPlayerInfo();
	// local.z += 45.0f; // compensation for not knowing where the head is...

	// todo: using entities ~~~~~~~~~~~~~~~~~~~~
	// localPlayer = gameManager_->getLocalPlayerInfo();
	// targetPlayer = gameManager_->getPlayerInfo(targetPlayerBase_);

	// dx_ = targetPlayer.x - localPlayer.x;
	// dy_ = targetPlayer.y - localPlayer.y;
	// dz_ = targetPlayer.z - localPlayer.z;
	// dr_ = sqrt(dy_ * dy_ + dx_ * dx_);
	// todo: using entities ~~~~~~~~~~~~~~~~~~~~

	dx_ = target.x - local.x;
	dy_ = target.y - local.y;
	dz_ = target.z - local.z;
	dr_ = sqrt(dy_ * dy_ + dx_ * dx_);

	// adjustment for division by 0 error
	dx_ += 0.00001f * (dx_ == 0.0f);
	dr_ += 0.00001f * (dr_ == 0.0f);

	xySlope = dy_ / dx_;
	rzSlope = dz_ / dr_;

	goalPitch_ = (-1.0f) * atan(rzSlope) * 180.0f / PI;
	goalYaw_ = atan(xySlope) * 180.0f / PI;

	goalYaw_ += 180.0f * (dx_ < 0); // Normalize angle

	// Contrict to bounds -180 < yaw < 180, -89 < pitch < 89
	goalPitch_ += ((-1.0f) * goalPitch_ + 89.0f) * (goalPitch_ > 89.0f) +
		((-1.0f) * goalPitch_ + -89.0f) * (goalPitch_ < -89.0f);
	goalYaw_ += 360.0f * (goalYaw_ < -180.0f) + -360.0f * (goalYaw_ > 180.0f);

	// todo: Applies smoothing. UNDER REDEVELOPEMENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	dPitch_ = goalPitch_ - local.pitch;
	dYaw_ = goalYaw_ - local.yaw;

	// todo: if delta yaw is between 360 and 180 degrees, we should rotate in the other direction
	dYaw_ += 360.0f * (dYaw_ < -180.0f)
		+ -360.0f * (dYaw_ > 180.0f);

	newPitch_ = local.pitch + dPitch_ / 2.0f;
	newYaw_ = local.yaw + dYaw_ / 2.0f;
	// todo: Applies smoothing. UNDER REDEVELOPEMENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (rageMode_)
	{
		injectAngle(goalPitch_, goalYaw_);
	}
	else
	{
		injectAngle(newPitch_, newYaw_);
		Sleep(10);
	}
}

void Tf2Tracking::injectAngle(float pitch, float yaw)
{
	const DWORD nBytes = 4;

	BYTE pitchAndYaw[2 * nBytes] = {0};

	DWORD dwPitch = *(DWORD*)&pitch;
	DWORD dwYaw = *(DWORD*)&yaw;

	for (DWORD i = 0; i < nBytes; ++i)
	{
		pitchAndYaw[i] = (dwPitch >> (i * 8)) & 0xFF;
		pitchAndYaw[i + nBytes] = (dwYaw >> (i * 8)) & 0xFF;
	}

	proc_->writeAddress(gameManager_->dwPitchBase(), pitchAndYaw,
	                           2 * nBytes);
}

void Tf2Tracking::printVerbose()
{
	// todo: figure out where to put this verbose shit
	if (goalPitch_ > 89.0f || goalPitch_ < -89.0f) printf("Pitch out of bounds\n");
	if (goalYaw_ > 180.0f || goalYaw_ < -180.0f) printf("Yaw out of bounds\n");


	std::cout << "goal pitch:" << goalPitch_ << "\n";
	std::cout << "goal yaw:" << goalYaw_ << "\n\n";

	std::cout << "delta pitch:" << dPitch_ << "\n";
	std::cout << "delta yaw:" << dYaw_ << "\n\n";

	std::cout << "new pitch:" << newPitch_ << "\n";
	std::cout << "new yaw:" << newYaw_ << "\n\n";
	std::cout << std::endl;
	// Sleep(1000);

	system("cls");

	// printPlayerInfo(localPlayer);
	// printPlayerInfo(targetPlayer);
	// std::cout << "dr_: " << dr_ << "\n";
	// std::cout << "dx_: " << dx_ << "\n";
	// std::cout << "dy_: " << dy_ << "\n";
	// std::cout << "dz_: " << dz_ << "\n";
	//
	// cout << "xySlope: " << xySlope << "\n\n";
	// cout << "yzSlope: " << rzSlope << "\n\n";
	// // Sleep(100);
	// system("cls");
}
