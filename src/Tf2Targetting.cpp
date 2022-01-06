#include "Tf2Targetting.h"

// todo: Create "Threaded" abstraction ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tf2Targetting::Tf2Targetting(Process* process, Tf2Tracking* tracking, DWORD localPlayerBase, DWORD entListBase)
{
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
	tracking_->pause();
	// if (paused_) return;
	// WaitForSingleObject(mutex_, INFINITE);
	// paused_ = true;
}

void Tf2Targetting::resume()
{
	tracking_->resume();
	// if (!paused_) return;
	// ReleaseMutex(mutex_);
	// paused_ = false;
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
	WaitForSingleObject(mutex_, INFINITE);
	while (1)
	{
		while (isGoodTarget(bestTargetBase_))
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
	BoneMatrix_t bonem;
	const float threshold = 200.0f;
	process_->readAddress(ent.boneMatrix, (BYTE*)&bonem, sizeof(bonem));

	float dx = ent.x5 - bonem.headx;
	float dy = ent.y5 - bonem.heady;
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

ClientInfo_t Tf2Targetting::getClientAtIndex(DWORD index)
{
	ClientInfo_t client;
	process_->readAddress(entityListBase_ + sizeof(ClientInfo_t) * index,
	                      (BYTE*)&client, sizeof(client));
	return client;
}
