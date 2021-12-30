#include "Tf2Aimbot.h"

// todo: remove this ~~~~~~~~~~~~~
#include <chrono>
using namespace std::chrono;
// todo: remove this ~~~~~~~~~~~~~

Tf2Aimbot::Tf2Aimbot()
{
	procManager_ = new ProcessManager();
	//todo: process is being initialized by GameOffset. Should be initialized here...
	// gameManager_ = new Tf2GameManager(procManager_);
	gameManager_ = new Tf2GameManagerV2(procManager_);
	mutex_ = CreateMutexA(NULL, TRUE, NULL);
	switchToNextTarget();
}

Tf2Aimbot::~Tf2Aimbot()
{
	closeTargettingThread();
	delete procManager_;
	delete gameManager_;
}

void Tf2Aimbot::start()
{
	openTargettingThread();
	resumeThread();

	char input = 0;

	while (1)
	{
		std::cin >> input;
		switch (input)
		{
		case 'N':
		case 'n':
			switchToNextTarget();
			break;
		case 'B':
		case 'b':
			switchToPrevTarget();
			break;
		case 'P':
		case 'p':
			try
			{
				pauseThread();
			}
			catch (const char* err)
			{
				cout << err << endl;
			}
			break;
		case 'R':
		case 'r':
			try
			{
				resumeThread();
			}
			catch (const char* err)
			{
				cout << err << endl;
			}
			break;
		case 'Q':
		case 'q':
			return;
			break;
		default:
			break;
		}
	}
	// todo: start the aimbot main loop for controlling lockons.
	// todo: Find out how to identify my own player.
}

void Tf2Aimbot::switchToNextTarget()
{
	ClientInfo client;
	if (!paused_) WaitForSingleObject(mutex_, INFINITE);

	do
	{
		targetPlayerIndex_ =
			(targetPlayerIndex_ + 1) >= MAX_PLAYERS ? 0 : targetPlayerIndex_ + 1;
		client = gameManager_->getClientAtIndex(targetPlayerIndex_);
	}
	while (!client.entity || client.entity == gameManager_->dwLocalPlayerBase());
	targetPlayerBase_ = client.entity;

	if (!paused_) ReleaseMutex(mutex_);
}

void Tf2Aimbot::switchToPrevTarget()
{
	ClientInfo_t client;
	if (!paused_) WaitForSingleObject(mutex_, INFINITE);

	do
	{
		targetPlayerIndex_ = targetPlayerIndex_ == 0
			                     ? MAX_PLAYERS - 1
			                     : targetPlayerIndex_ - 1;

		client = gameManager_->getClientAtIndex(targetPlayerIndex_);
	}
	while (!client.entity || client.entity == gameManager_->dwLocalPlayerBase());
	targetPlayerBase_ = client.entity;

	if (!paused_) ReleaseMutex(mutex_);
}


void Tf2Aimbot::pauseThread()
{
	if (paused_)
		throw "(Tf2Aimbot/pauseThread) Thread already paused";
	WaitForSingleObject(mutex_, INFINITE);
	paused_ = true;
}

void Tf2Aimbot::resumeThread()
{
	if (!paused_)
		throw "(Tf2Aimbot/resumeThread) Not currently paused";

	if (!ReleaseMutex(mutex_))
		throw "(Tf2Aimbot/resumeThread) Releasing mutex twice";
	paused_ = false;
}

void Tf2Aimbot::closeTargettingThread()
{
	if (!paused_)
		WaitForSingleObject(mutex_, INFINITE);
	terminateThread_ = true;
	ReleaseMutex(mutex_);
	WaitForSingleObject(thread_, INFINITE);

	CloseHandle(thread_); // todo: check if this thread is even open
	CloseHandle(mutex_);
}

DWORD WINAPI trackPlayerThread(LPVOID vParams)
{
	auto params = (ThreadParam*)vParams;
	params->obj_->initThread(); // todo: change this to execute tracking
	delete params;
	return 0;
}


void Tf2Aimbot::openTargettingThread()
{
	// todo: make the thread control the cursor. Pass params through the thread as well.
	auto params = new ThreadParam(this, 0, 1);
	thread_ = CreateThread(NULL, 0, trackPlayerThread, (LPVOID)params, 0, NULL);
}

void Tf2Aimbot::initThread()
{
	while (1)
	{
		if (WAIT_FAILED == WaitForSingleObject(mutex_, INFINITE))
			throw "(tf2Aimbot/testThread) wait failed";

		if (terminateThread_)
		{
			cout << "TERMINATING!!!!\n";
			ReleaseMutex(mutex_);
			return;
		}

		trackPlayer();

		ReleaseMutex(mutex_);
	}
}


void Tf2Aimbot::trackPlayer()
{
	PlayerInfo_t player;
	PlayerInfo_t bot1;

	float dx, dy, dr, dz;
	float xySlope, rzSlope;
	float pitch, yaw;

	player = gameManager_->getLocalPlayerInfo();
	bot1 = gameManager_->getPlayerInfo(targetPlayerBase_);

	dx = bot1.x - player.x;
	dy = bot1.y - player.y;
	dz = bot1.z - player.z;
	dr = sqrt(dy * dy + dx * dx);

	xySlope = dy / dx;
	rzSlope = dz / dr;

	// todo: Figure out why we have to invert pitch...
	pitch = (-1) * atan(rzSlope) * 180 / PI;
	yaw = atan(xySlope) * 180 / PI;

	// todo: the bound restrictions for tan...
	yaw += 180 * (dx < 0);


	injectAngle(pitch, yaw);

	// todo: figure out where to put this verbose shit
	// printPlayerInfo(player);
	// printPlayerInfo(bot1);
	// cout << "dr: " << dr << "\n";
	// cout << "dx: " << dx << "\n";
	// cout << "dy: " << dy << "\n";
	// cout << "dz: " << dz << "\n";
	//
	// cout << "xySlope: " << xySlope << "\n\n";
	// cout << "yzSlope: " << rzSlope << "\n\n";
	// // Sleep(100);
	// system("cls");
}

void Tf2Aimbot::injectAngle(float pitch, float yaw)
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

	procManager_->writeAddress(gameManager_->dwPitchBase(),
	                           pitchAndYaw,
	                           2 * nBytes);
}

void Tf2Aimbot::printPlayerInfo(PlayerInfo_t player)
{
	// cout << "Player hp: " << player.hp << "\n";
	printf("Player hp: %d\n", player.hp);
	cout << "Player x: " << player.x << "\n";
	cout << "Player y: " << player.y << "\n";
	cout << "Player z: " << player.z << "\n";
	cout << "Player observer mode: " << player.observermode << "\n";
	cout << "Player cursorid: " << player.cursorid << "\n";
	cout << endl;
}
