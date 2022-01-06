#include "Tf2Aimbot.h"

// todo: remove this ~~~~~~~~~~~~~
#include <chrono>
using namespace std::chrono;
// todo: remove this ~~~~~~~~~~~~~

Tf2Aimbot::Tf2Aimbot()
{
	procManager_ = new Process();
	gameManager_ = new Tf2OffsetV2(procManager_);
	tracking_ = new Tf2Tracking(procManager_, gameManager_);
	targetting_ = new Tf2Targetting(procManager_, tracking_, gameManager_->dwLocalPlayerBase(),
	                                gameManager_->entityListBase());
	// updateTarget();
}

Tf2Aimbot::~Tf2Aimbot()
{
	delete tracking_;
	delete procManager_;
	delete gameManager_;
}

void Tf2Aimbot::start()
{
	const DWORD sleepTime = 250;
	cout << "Caps: enable lockon\nZ: switch to previous target\nX: Switch to next target\nAlt: toggle aim smoothing\n";
	while (1)
	{
		if (GetAsyncKeyState(VK_CAPITAL))
		{
			if (tracking_->isPaused())
				tracking_->resume();
			else
				tracking_->pause();

			Sleep(sleepTime);
		}
		if (GetAsyncKeyState(0x5A)) // Z key
		{
			// switchToPrevTarget();
			targetting_->extSwitchToPrevTarget();
			// updateTarget();
			Sleep(sleepTime);
		}
		if (GetAsyncKeyState(0x58)) // X key
		{
			targetting_->extSwitchToNextTarget();
			// updateTarget();
			Sleep(sleepTime);
		}
		if (GetAsyncKeyState(VK_MENU)) // alt
		{
			tracking_->toggleRageMode();
			Sleep(sleepTime);
		}
	}
}

// void Tf2Aimbot::updateTarget()
// {
// 	// switchTarget(nextIndex);
// 	tracking_->setTargetBase(targetting_->bestTargetBase());
// }

//
// void Tf2Aimbot::switchToPrevTarget() { switchTarget(prevIndex); };
//
// void Tf2Aimbot::switchTarget(void (*getNewTarget)(Tf2Aimbot*))
// {
// 	ClientInfo_t client;
//
// 	do
// 	{
// 		getNewTarget(this);
// 		client = gameManager_->getClientAtIndex(targetPlayerIndex_);
// 	}
// 	while (!isGoodTarget(client));
// 	tracking_->setTargetBase(client.entity);
// }
//
// bool Tf2Aimbot::isGoodTarget(ClientInfo_t client)
// {
// 	if (!client.entity || client.entity == gameManager_->dwLocalPlayerBase())
// 		return false;
//
// 	Entity_t ent = {0};
// 	Entity_t localPlayer = {0};
// 	procManager_->readAddress(client.entity, (BYTE*)&ent, sizeof(ent));
// 	procManager_->readAddress(gameManager_->dwLocalPlayerBase(), (BYTE*)&localPlayer, sizeof(localPlayer));
// 	return !ent.deadFlag && ent.teamNo >= 2 && ent.teamNo != localPlayer.teamNo;
// }
//
// void Tf2Aimbot::nextIndex(Tf2Aimbot* obj)
// {
// 	DWORD newIndex = (obj->targetPlayerIndex() + 1) >= MAX_PLAYERS
// 		                 ? 0
// 		                 : obj->targetPlayerIndex() + 1;
// 	obj->setTargetPlayerIndex(newIndex);
// }
//
// void Tf2Aimbot::prevIndex(Tf2Aimbot* obj)
// {
// 	DWORD newIndex = obj->targetPlayerIndex() == 0
// 		                 ? MAX_PLAYERS - 1
// 		                 : obj->targetPlayerIndex() - 1;
// 	obj->setTargetPlayerIndex(newIndex);
// }

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
