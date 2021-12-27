#include "Tf2Aimbot.h"


Tf2Aimbot::Tf2Aimbot()
{
	procManager_ = new ProcessManager();
	//todo: process is being initialized by GameOffset. Should be initialized here...
	offsets_ = new GameOffsetsTf2(procManager_);
}

Tf2Aimbot::~Tf2Aimbot()
{
}

void Tf2Aimbot::testTrackBot1()
{
	PlayerInfo_t player;
	PlayerInfo_t bot1;

	float dx, dy, dt, dz;
	float xySlope, tzSlope;
	float pitch, yaw;

	while (1)
	{
		system("cls");

		player = offsets_->getPlayerInfo(0);
		bot1 = offsets_->getPlayerInfo(3);

		printPlayerInfo(player);
		printPlayerInfo(bot1);

		dx = bot1.x - player.x;
		dy = bot1.y - player.y;
		dz = bot1.z - player.z;

		dt = dx * (dx > dy) + dy * (!(dx > dy));

		xySlope = dy / dx;
		tzSlope = dz / dt;


		cout << "dx: " << dx << "\n";
		cout << "dy: " << dy << "\n";
		cout << "dz: " << dz << "\n";

		cout << "xySlope: " << xySlope << "\n\n";
		cout << "yzSlope: " << tzSlope << "\n\n";

		// if (xySlope > 0) xySlope += 180;

		pitch = atan(tzSlope) * 180 / PI;
		yaw = atan(xySlope) * 180 / PI;

		// todo: the bound restrictions for tan...
		yaw += 180 * (dx < 0);
		// todo: Figure out why we have to invert pitch...
		pitch *= -1;

		injectAngle(pitch, yaw);

		// Sleep(100);
	}
}

void Tf2Aimbot::injectAngle(float pitch, float yaw)
{
	// DWORD const nBytes = sizeof(float);
	DWORD const nBytes = 4;
	BYTE pitchBytes[nBytes] = {0};
	BYTE yawBytes[nBytes] = {0};

	DWORD dwPitch = *(DWORD*)&pitch;
	DWORD dwYaw = *(DWORD*)&yaw;

	DWORD baseEngine_dll = offsets_->dwEngine();

	for (DWORD i = 0; i < nBytes; ++i)
	{
		pitchBytes[i] = (dwPitch >> (i * 8)) & 0xFF;
		yawBytes[i] = (dwYaw >> (i * 8)) & 0xFF;
	}

	// printf("dwPitch: %x\n", dwPitch);
	// printf("dwYaw: %x\n", dwYaw);
	//
	// cout << "pitchBytes: 0x";
	// for (DWORD i = 0; i < nBytes; ++i)
	// {
	// 	printf("%x ", pitchBytes[i]);w 
	// }
	// cout << endl;
	//
	// cout << "yawBytes: 0x";
	// for (DWORD i = 0; i < nBytes; ++i)
	// {
	// 	printf("%x ", yawBytes[i]);
	// }
	// cout << endl;
	// system("pause");

	// procManager_->writeAddress(baseEngine_dll + PITCH_OFFSET,
	//                            pitchBytes,
	//                            nBytes);

	procManager_->writeAddress(baseEngine_dll + YAW_OFFSET,
	                           yawBytes,
	                           nBytes);
}

void Tf2Aimbot::printPlayerInfo(PlayerInfo_t player)
{
	cout << "Player hp: " << player.hp << "\n";
	cout << "Player x: " << player.x << "\n";
	cout << "Player y: " << player.y << "\n";
	cout << "Player z: " << player.z << "\n";
	cout << endl;
}
