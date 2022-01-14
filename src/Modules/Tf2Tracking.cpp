#include "Tf2Tracking.h"

// todo: Find eye position, and use that for local player position in tracking
// system. (find view-matrix) todo: Check if player is on my team before
// choosing a target todo: Check if player is dead before choosing target
// todo:Add targetting FOV todo: todo:

Tf2Tracking::Tf2Tracking(Process* procManager, IOffset* offsets, Local* local, DWORD engineBase)
{
	local_ = local;
	process_ = procManager;
	gameManager_ = offsets;
	engineBase_ = engineBase;
	mutex_ = CreateMutexA(NULL, TRUE, NULL);
	openThread();
}

Tf2Tracking::~Tf2Tracking()
{
	closeThread();
	CloseHandle(thread_);
	CloseHandle(mutex_);
}

const DWORD boneBaseOffset = 0x5B0;

void Tf2Tracking::setTargetBase(DWORD newTargetBase)
{
	if (paused_)
	{
		targetPlayerBase_ = newTargetBase;
		targetBoneBase_ = process_->read<DWORD>(targetPlayerBase_ + boneBaseOffset);
		// todo: This shouldn't be static...
	}
	else
	{
		pause();
		targetPlayerBase_ = newTargetBase;
		targetBoneBase_ = process_->read<DWORD>(targetPlayerBase_ + boneBaseOffset);
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
	paused_ = false;
	if (!ReleaseMutex(mutex_))
		throw "(Tf2Aimbot/resumeThread) Releasing mutex twice";
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

/**
 * ID's
 * Belt:0
 * head:6
 */
Vec3_t Tf2Tracking::getTargetCoordinate()
{
	// todo: maybe this shouldnt be handled here? hmm...
	Vec3_t result = {0};
	BoneMatrix_t bonem = {0};
	DWORD bodyId = 6;

	process_->readAddress(targetBoneBase_ + sizeof(bonem) * bodyId, (BYTE*)&bonem,
	                      sizeof(bonem));
	result.x = bonem.x;
	result.y = bonem.y;
	result.z = bonem.z;
	return result;
}

Vec3_t Tf2Tracking::getPlayerCoordinate(DWORD playerBase)
{
	// todo: maybe this shouldnt be handled here? hmm...
	Vec3_t result = {0};
	BoneMatrix_t bonem = {0};
	DWORD bodyId = 6; // engie is 8?? demo is 16
	// todo: this shouldnt be static...

	DWORD boneBase = process_->read<DWORD>(playerBase + boneBaseOffset);
	process_->readAddress(boneBase + sizeof(bonem) * bodyId, (BYTE*)&bonem,
	                      sizeof(bonem));

	result.x = bonem.x;
	result.y = bonem.y;
	result.z = bonem.z;
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
	// Vec3_t target;
	LocalCoords_t local = local_->getLocalCoordinate();
	Angle_t angle = {0};
	const float m = 4.0f;

	// todo: Applies smoothing. UNDER REDEVELOPEMENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (rageMode_)
	{
		angle = Geometry_.getAbsoluteAngle(getTargetCoordinate(), local);
		injectAngle(angle.pitch, angle.yaw);
	}
	else
	{
		angle = Geometry_.getRelativeAngle(getTargetCoordinate(), local);


		while (angle.pitch >= 35.f || angle.pitch <= -35.f)
		{
			angle.pitch /= m;
		}

		while (angle.yaw >= 35.f || angle.yaw <= -35.f)
		{
			angle.yaw /= m;
		}

		if (angle.yaw >= 35.f || angle.yaw <= -35.f ||
			angle.pitch >= 35.f || angle.pitch <= -35.f)
		{
			throw "SPEED ERROR";
		}

		newPitch_ = local.pitch + angle.pitch / smoothFactor_;
		newYaw_ = local.yaw + angle.yaw / smoothFactor_;

		if (newYaw_ <= -180.f) newYaw_ += 360.f;
		else if (newYaw_ >= 180.f) newYaw_ -= 360.f;

		if (newPitch_ <= -90.f) newPitch_ += 180.f;
		else if (newPitch_ >= 90.f) newPitch_ -= 180.f;


		if (newYaw_ >= 180.f || newYaw_ <= -180.f ||
			newPitch_ > 89.f || newPitch_ < -89.f)
		{
			throw "CLAMP ERROR";
		}

		injectAngle(newPitch_, newYaw_);
		// Sleep(10);
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

	process_->writeAddress(gameManager_->pitchBase(), pitchAndYaw, 2 * nBytes);
}
