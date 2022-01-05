#pragma once
#include "IGameManager.h"
#include "ProcessManager.h"

#define PITCH_OFFSET 0x467474
#define YAW_OFFSET PITCH_OFFSET + 4
#define MAX_PLAYERS 24

using Entity_t = struct Entity
{ //todo: weird how A7 padding fucks things up...
public:
	char pad_0000[165]; // 0x0000
	WORD deadFlag; // 0x00A5
	// char pad_00A7[1]; // 0x00A7
	DWORD hp; // 0x00A8
	char pad_00AC[4]; // 0x00AC
	DWORD teamNo; // 0x00B0
	char pad_00B4[472]; // 0x00B4
	float x; // 0x028C
	float y; // 0x0290
	float z; // 0x0294
	float pitch1; // 0x0298
	float yaw1; // 0x029C
	float roll1; // 0x02A0
	float x2; // 0x02A4
	float y2; // 0x02A8
	float z2; // 0x02AC
	char pad_02B0[12]; // 0x02B0
	float x3; // 0x02BC
	float y3; // 0x02C0
	float z3; // 0x02C4
	char pad_02C8[48]; // 0x02C8
	float vecorigPitch2; // 0x02F8
	float vecorigYaw2; // 0x02FC
	float vecorigRoll2; // 0x0300
	char pad_0304[60]; // 0x0304
	float x4; // 0x0340
	char pad_0344[12]; // 0x0344
	float y4; // 0x0350
	char pad_0354[12]; // 0x0354
	float z4; // 0x0360
	float x5; // 0x0364
	float y5; // 0x0368
	float z5; // 0x036C
	char pad_0370[576]; // 0x0370
	DWORD boneMatrixBase; // 0x05B0
}; // Size: 0x05B4


// using BoneMatrix_t = struct BoneMatrix
// {
// public:
// 	BYTE pad0[12];
// 	float headx;
// 	BYTE pad1[12];
// 	float heady;
// 	BYTE pad2[12];
// 	float headz;
// }; // Size: 0x05B4

class Tf2GameManagerV2 : public IGameManager
{
public:
	Tf2GameManagerV2(ProcessManager* proc);

	~Tf2GameManagerV2() override
	{
	};

	void printVerbose() override;
	PlayerInfo_t getPlayerInfo(DWORD playerBase) override;
	PlayerInfo_t getLocalPlayerInfo() override;

	DWORD dwPitchBase() const override { return (DWORD)pitchBase_; };
	DWORD dwYawBase() const override { return (DWORD)yawBase_; };
	DWORD dwLocalPlayerBase() const override { return localPlayerBase_; };

	ClientInfo_t getClientAtIndex(DWORD index) override;


	void testPrintHealths();

private:
	ProcessManager* procManager_ = 0;

	DWORD clientBase_ = 0;
	DWORD engineBase_ = 0;

	DWORD entityListBase_ = 0;
	DWORD localPlayerBase_ = 0;

	float* pitchBase_ = 0;
	float* yawBase_ = 0;

	DWORD clientModSize_ = 0;
	DWORD engineModSize_ = 0;

	void getLocalPlayerBase();
	void loadProcessAndModules();
	void getEntityListBasePtr();
	PlayerInfo_t loadPlayerInfo(DWORD entityBase);
	PlayerInfo_t loadLocalPlayerInfo(DWORD entityBase);
};
