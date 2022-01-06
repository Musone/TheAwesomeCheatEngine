#pragma once
#include <windows.h>

typedef struct PlayerInfo
{
	// offset
	DWORD hp; // 0xAC
	float x; // 0x28C
	float y; // 0x290
	float z; // 0x294
	DWORD observermode; // 0x109C
	DWORD cursorid; // 0x177C
	float pitch;
	float yaw;
} PlayerInfo_t;

typedef struct ClientInfo
{
	DWORD entity;
	DWORD SerialNumber;
	DWORD prev;
	DWORD next;
} ClientInfo_t;

using BoneMatrix_t = struct BoneMatrix
{
public:
	BYTE pad0[12];
	float headx;
	BYTE pad1[12];
	float heady;
	BYTE pad2[12];
	float headz;
}; // Size: 0x05B4

using Entity_t = struct Entity // todo: weird how A7 padding fucks things up...
{
public:
	BYTE pad_0000[164]; // 0x0000
	DWORD deadFlag; // 0x00A4  will be 0x202 if dead.
	DWORD hp; // 0x00A8
	BYTE pad_00AC[4]; // 0x00AC
	DWORD teamNo; // 0x00B0
	BYTE pad_00B4[472]; // 0x00B4
	float x; // 0x028C
	float y; // 0x0290
	float z; // 0x0294
	float pitch1; // 0x0298
	float yaw1; // 0x029C
	float roll1; // 0x02A0
	float x2; // 0x02A4
	float y2; // 0x02A8
	float z2; // 0x02AC
	BYTE pad_02B0[12]; // 0x02B0
	float x3; // 0x02BC
	float y3; // 0x02C0
	float z3; // 0x02C4
	BYTE pad_02C8[48]; // 0x02C8
	float vecorigPitch2; // 0x02F8
	float vecorigYaw2; // 0x02FC
	float vecorigRoll2; // 0x0300
	BYTE pad_0304[60]; // 0x0304
	float x4; // 0x0340
	char pad_0344[12]; // 0x0344
	float y4; // 0x0350
	BYTE pad_0354[12]; // 0x0354
	float z4; // 0x0360
	float x5; // 0x0364
	float y5; // 0x0368
	float z5; // 0x036C
	BYTE pad_0370[576]; // 0x0370
	DWORD boneMatrix; // 0x05B0
}; // Size: 0x05B4


// using Entity_t = struct Entity {  // todo: weird how A7 padding fucks things up...
//  public:
//   char pad_0000[165];  // 0x0000
//   WORD deadFlag;       // 0x00A5
//   // char pad_00A7[1]; // 0x00A7
//   DWORD hp;              // 0x00A8
//   char pad_00AC[4];      // 0x00AC
//   DWORD teamNo;          // 0x00B0
//   char pad_00B4[472];    // 0x00B4
//   float x;               // 0x028C
//   float y;               // 0x0290
//   float z;               // 0x0294
//   float pitch1;          // 0x0298
//   float yaw1;            // 0x029C
//   float roll1;           // 0x02A0
//   float x2;              // 0x02A4
//   float y2;              // 0x02A8
//   float z2;              // 0x02AC
//   char pad_02B0[12];     // 0x02B0
//   float x3;              // 0x02BC
//   float y3;              // 0x02C0
//   float z3;              // 0x02C4
//   char pad_02C8[48];     // 0x02C8
//   float vecorigPitch2;   // 0x02F8
//   float vecorigYaw2;     // 0x02FC
//   float vecorigRoll2;    // 0x0300
//   char pad_0304[60];     // 0x0304
//   float x4;              // 0x0340
//   char pad_0344[12];     // 0x0344
//   float y4;              // 0x0350
//   char pad_0354[12];     // 0x0354
//   float z4;              // 0x0360
//   float x5;              // 0x0364
//   float y5;              // 0x0368
//   float z5;              // 0x036C
//   char pad_0370[576];    // 0x0370
//   DWORD boneMatrixBase;  // 0x05B0
// };                       // Size: 0x05B4
