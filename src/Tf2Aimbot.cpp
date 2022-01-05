#include "Tf2Aimbot.h"

// todo: remove this ~~~~~~~~~~~~~
#include <chrono>
using namespace std::chrono;
// todo: remove this ~~~~~~~~~~~~~

Tf2Aimbot::Tf2Aimbot()
{
	procManager_ = new ProcessManager();
	gameManager_ = new Tf2GameManagerV2(procManager_);
	trackingSystem_ = new TrackingSystem(procManager_, gameManager_);
	switchToNextTarget();
}

Tf2Aimbot::~Tf2Aimbot()
{
	delete trackingSystem_;
	delete procManager_;
	delete gameManager_;
}

void Tf2Aimbot::resumeTracking() { trackingSystem_->resume(); }

void Tf2Aimbot::pauseTracking() { trackingSystem_->pause(); }

void Tf2Aimbot::switchToNextTarget() { switchTarget(nextIndex); }

void Tf2Aimbot::switchToPrevTarget() { switchTarget(prevIndex); };

void Tf2Aimbot::start()
{
	const DWORD sleepTime = 250;
	cout << "Press caps to lock, and use the arrow keys to switch targets.\n";
	while (1)
	{
		if (GetAsyncKeyState(VK_CAPITAL))
		{
			if (trackingSystem_->isPaused())
				resumeTracking();
			else
				pauseTracking();

			Sleep(sleepTime);
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			switchToPrevTarget();
			Sleep(sleepTime);
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			switchToNextTarget();
			Sleep(sleepTime);
		}
	}
}

void Tf2Aimbot::switchTarget(void (*getNewTarget)(Tf2Aimbot*))
{
	ClientInfo_t client;

	do
	{
		getNewTarget(this);
		client = gameManager_->getClientAtIndex(targetPlayerIndex_);
	}
	while (!isGoodTarget(client));
	trackingSystem_->setTargetBase(client.entity);
}

bool Tf2Aimbot::isGoodTarget(ClientInfo_t client)
{
	if (!client.entity || client.entity == gameManager_->dwLocalPlayerBase())
		return false;

	Entity_t ent = {0};
	Entity_t localPlayer = {0};
	procManager_->readAddress(client.entity, (BYTE*)&ent, sizeof(ent));
	procManager_->readAddress(gameManager_->dwLocalPlayerBase(), (BYTE*)&localPlayer, sizeof(localPlayer));
	return !ent.deadFlag && ent.teamNo >= 2 && ent.teamNo != localPlayer.teamNo;
}

void Tf2Aimbot::nextIndex(Tf2Aimbot* obj)
{
	DWORD newIndex = (obj->targetPlayerIndex() + 1) >= MAX_PLAYERS
		                 ? 0
		                 : obj->targetPlayerIndex() + 1;
	obj->setTargetPlayerIndex(newIndex);
}

void Tf2Aimbot::prevIndex(Tf2Aimbot* obj)
{
	DWORD newIndex = obj->targetPlayerIndex() == 0
		                 ? MAX_PLAYERS - 1
		                 : obj->targetPlayerIndex() - 1;
	obj->setTargetPlayerIndex(newIndex);
}

void Tf2Aimbot::printPlayerInfo(PlayerInfo_t player)
{
	// cout << "Player hp: " << localPlayer.hp << "\n";
	printf("Player hp: %d\n", player.hp);
	cout << "Player x: " << player.x << "\n";
	cout << "Player y: " << player.y << "\n";
	cout << "Player z: " << player.z << "\n";
	cout << "Player observer mode: " << player.observermode << "\n";
	cout << "Player cursorid: " << player.cursorid << "\n";
	cout << endl;
}
