#include "Tf2Targetting.h"

// todo: Create "Threaded" abstraction ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// todo: Abstract class for all threaded classes to inherit from
Tf2Targetting::Tf2Targetting(IOffset* offsets, Process* process,
                             Tf2Tracking* tracking, Local* local,
                             Screen* screen,
                             DWORD localPlayerBase, DWORD entListBase)
{
	offsets_ = offsets;
	local_ = local;
	screen_ = screen;
	process_ = process;
	tracking_ = tracking;
	entityListBase_ = entListBase;
	localPlayerBase_ = localPlayerBase;
	mutex_ = CreateMutexA(NULL, FALSE, NULL);
	openThread();
}


Tf2Targetting::~Tf2Targetting()
{
	closeThread();
	CloseHandle(thread_);
	CloseHandle(mutex_);
}


DWORD WINAPI Tf2Targetting::threadInit(LPVOID vthis)
{
	Tf2Targetting* obj = (Tf2Targetting*)vthis;
	obj->mainLoop();
	return 0;
}


void Tf2Targetting::closeThread()
{
	pause();
	terminateThread_ = true;
	resume();
	WaitForSingleObject(thread_, INFINITE);
}

void Tf2Targetting::openThread()
{
	thread_ = CreateThread(NULL, 0, threadInit, (LPVOID)this, 0, NULL);
}

void Tf2Targetting::pause()
{
	// tracking_->pause();
	if (paused_) return;
	WaitForSingleObject(mutex_, INFINITE);
	printf("Auto re-Targetting paused\n");
	paused_ = true;
}

void Tf2Targetting::resume()
{
	// tracking_->resume();
	if (!paused_) return;
	paused_ = false;
	printf("Auto re-Targetting resumed\n");
	ReleaseMutex(mutex_);
}

// todo: duplicate code between this and Tf2Tracking
// todo: Create "Threaded" abstraction ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Tf2Targetting::extSwitchToNextTarget()
{
	WaitForSingleObject(mutex_, INFINITE);
	switchToNextTarget();
	ReleaseMutex(mutex_);
}

void Tf2Targetting::extSwitchToPrevTarget()
{
	WaitForSingleObject(mutex_, INFINITE);
	switchToPrevTarget();
	ReleaseMutex(mutex_);
}


void Tf2Targetting::mainLoop()
{
	// todo: use fov maybe??
	WaitForSingleObject(mutex_, INFINITE);
	while (!terminateThread_)
	{
		while (isGoodTarget(bestTargetBase_) && !terminateThread_)
		{
			// todo: this should switch after killing target
			ReleaseMutex(mutex_);
			Sleep(750);
			WaitForSingleObject(mutex_, INFINITE);
		};
		switchToNextTarget();
	}
}

void Tf2Targetting::switchToNextTarget()
{
	switchTarget(nextIndex);
	tracking_->setTargetBase(bestTargetBase_);
}

void Tf2Targetting::switchToPrevTarget()
{
	switchTarget(prevIndex);
	tracking_->setTargetBase(bestTargetBase_);
};

void Tf2Targetting::switchTarget(void (*getNewTarget)(Tf2Targetting*))
{
	ClientInfo_t client;

	do
	{
		getNewTarget(this);
		client = getClientAtIndex(targetPlayerIndex_);
	}
	while (!isGoodTarget(client.entity));
	bestTargetBase_ = client.entity;
}

// todo: fix duplicate code....
bool Tf2Targetting::targetPlayerInFov()
{
	LocalCoords_t local = local_->getLocalCoordinate();
	ClientInfo_t client = {0};
	Entity_t playerEnt = {0};
	DWORD newBestTargetBase = 0;
	float shortestDistance = -1.0f;
	float newDistance = -1.0f;
	DWORD newTargetIndex = 0;
	Vec3_t playerPos = {0};

	for (DWORD i = 0; i < MAX_PLAYERS; ++i)
	{
		client = getClientAtIndex(i);
		//todo: use W2S to check if he is in FOV;
		if (isGoodTarget(client.entity))
		{
			// printf("is good target\n");
			process_->readAddress(client.entity, (BYTE*)&playerEnt, sizeof(playerEnt));
			playerPos = {playerEnt.x5, playerEnt.y5, playerEnt.z5 + Z_COMPENSATION};


			if (isInFov(playerPos))
			{
				newDistance = Geometry_.calculateDistance(
					{local.x, local.y, local.z}, playerPos);

				if (shortestDistance < 0 ||
					newDistance < shortestDistance)
				{
					newBestTargetBase = client.entity;
					shortestDistance = newDistance;
					newTargetIndex = i;
				}
			}
		}
	}

	if (newBestTargetBase)
	{
		bestTargetBase_ = newBestTargetBase;
		targetPlayerIndex_ = newTargetIndex;
		tracking_->setTargetBase(bestTargetBase_);
		return true;
	}
	return false;
}

bool Tf2Targetting::isInFov(Vec3_t playerPos)
{
	int halfWidth = screen_->width_ / 2;
	int halfHeight = screen_->height_ / 2;
	int halfFov = screen_->aimbotFov_ / 2;

	int x1 = halfWidth - halfFov;
	int y1 = halfHeight - halfFov;

	int x2 = halfWidth + halfFov;
	int y2 = halfHeight + halfFov;

	float viewMatrix[16] = {0};
	process_->readAddress(offsets_->engineBase() + VIEW_MATRIX_OFFSET, (BYTE*)&viewMatrix, sizeof(viewMatrix));
	Vec2_t pos = Geometry_.worldToScreen(playerPos, (float*)viewMatrix, screen_);


	return pos.x >= x1 && pos.x <= x2 &&
		pos.y >= y1 && pos.y <= y2; //stub
}


bool Tf2Targetting::isGoodTarget(DWORD targetBase)
{
	// todo: check origin vs bone position
	if (!targetBase || targetBase == localPlayerBase_)
		return false;

	Entity_t ent = {0};
	Entity_t localPlayer = {0};
	process_->readAddress(targetBase, (BYTE*)&ent, sizeof(ent));
	process_->readAddress(localPlayerBase_, (BYTE*)&localPlayer, sizeof(localPlayer));

	if (ent.deadFlag == 0x202 ||
		ent.teamNo < 2 ||
		ent.teamNo > 3 ||
		ent.teamNo == localPlayer.teamNo)
	{
		return false;
	}

	// Check distance between bone and origin.
	BoneMatrix_t bonem = {0};
	const float threshold = 150.0f;
	process_->readAddress(ent.boneMatrix, (BYTE*)&bonem, sizeof(bonem));

	float dx = ent.x5 - bonem.x;
	float dy = ent.y5 - bonem.y;
	float distance = sqrt(dx * dx + dy * dy);

	return distance <= threshold;
}

void Tf2Targetting::nextIndex(Tf2Targetting* obj)
{
	DWORD newIndex = (obj->targetPlayerIndex() + 1) >= MAX_PLAYERS
		                 ? 0
		                 : obj->targetPlayerIndex() + 1;
	obj->setTargetPlayerIndex(newIndex);
}

void Tf2Targetting::prevIndex(Tf2Targetting* obj)
{
	DWORD newIndex = obj->targetPlayerIndex() == 0
		                 ? MAX_PLAYERS - 1
		                 : obj->targetPlayerIndex() - 1;
	obj->setTargetPlayerIndex(newIndex);
}

// todo: Duplicate code with Paint
ClientInfo_t Tf2Targetting::getClientAtIndex(DWORD index)
{
	ClientInfo_t client;
	process_->readAddress(entityListBase_ + sizeof(ClientInfo_t) * index,
	                      (BYTE*)&client, sizeof(client));
	return client;
}
