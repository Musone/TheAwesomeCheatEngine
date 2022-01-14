#include "Tf2Hacks.h"

// #include "../Overlay/overlay old/Tf2OverlayOld.h"

Tf2Hacks::Tf2Hacks(HINSTANCE hInstance, int nCmdShow)
{
	screen_ = new Screen();
	process_ = new Process();
	offsets_ = new Tf2OffsetV2(process_);
	local_ = new Local(process_, offsets_);
	tracking_ = new Tf2Tracking(process_, offsets_, local_, offsets_->engineBase());
	targetting_ = new Tf2Targetting(offsets_, process_, tracking_, local_, screen_, offsets_->localPlayerBase(),
	                                offsets_->entityListBase());


	overlay_ = new Tf2Overlay(offsets_, process_, screen_, hInstance, nCmdShow);
	// OverLayThreadParams_t params = {
	// 	offsets_,
	// 	process_,
	// 	screen_,
	// 	hInstance,
	// 	nCmdShow,
	// 	offsets_->entityListBase(),
	// 	offsets_->localPlayerBase(),
	// };
	// espThread_ =
	// 	CreateThread(NULL, 0, overlayThreadInit, (LPVOID)&params, 0, NULL);


	// updateTarget();
}

Tf2Hacks::~Tf2Hacks()
{
	// todo: update deconstructors
	// CloseHandle(espThread_);
	delete targetting_;
	delete tracking_;
	delete local_;
	delete offsets_;
	delete process_;
	delete overlay_;
	delete screen_;
}

void Tf2Hacks::start()
{
	const DWORD sleepTime = 250;
	// cout << "Caps: enable lockon\nZ: switch to previous target\nX: Switch to next target\nAlt: toggle aim smoothing\n";
	targetting_->pause();
	while (1)
	{
		if (GetAsyncKeyState(VK_CAPITAL))
		{
			// if (tracking_->isPaused() && targetting_->targetClosestPlayer())
			if (tracking_->isPaused() && targetting_->targetPlayerInFov())
			{
				// 
				tracking_->resume();
			}
			else
			{
				if (tracking_->isRageMode())
				{
					tracking_->toggleRageMode();
				}
				tracking_->pause();
			}


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
		if (GetAsyncKeyState(VK_NUMPAD1)) // alt
		{
			if (overlay_->isPaused())
			{
				overlay_->resume();
			}
			else
				overlay_->pause();
			Sleep(sleepTime);
		}
		// if (GetAsyncKeyState(0x50)) // P key
		// {
		// if (targetting_->isPaused())
		// 		targetting_->resume();
		// 	else
		// 		targetting_->pause();
		//
		// 	Sleep(sleepTime);
		// }
	}
}

void Tf2Hacks::printPlayerInfo(PlayerInfo_t player)
{
	// cout << "Player hp: " << localPlayer.hp << "\n";
	// printf("Player hp: %d\n", player.hp);
	// cout << "Player x: " << player.x << "\n";
	// cout << "Player y: " << player.y << "\n";
	// cout << "Player z: " << player.z << "\n";
	// cout << "Player observer mode: " << player.observermode << "\n";
	// cout << "Player cursorid: " << player.cursorid << "\n";
	// cout << endl;
}
