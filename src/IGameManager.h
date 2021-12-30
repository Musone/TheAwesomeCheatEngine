#pragma once


#include "windows.h"

typedef struct PlayerInfo
{
	// offset
	DWORD hp; // 0xAC
	float x; // 0x28C
	float y; // 0x290
	float z; // 0x294
	DWORD observermode; // 0x109C
	DWORD cursorid; // 0x177C
} PlayerInfo_t;

typedef struct ClientInfo
{
	DWORD entity;
	DWORD SerialNumber;
	DWORD prev;
	DWORD next;
} ClientInfo_t;

class IGameManager
{
public:
	virtual ~IGameManager()
	{
	};

	virtual void printVerbose() = 0;
	virtual PlayerInfo_t getPlayerInfo(DWORD index) = 0;
	virtual PlayerInfo_t getLocalPlayerInfo() = 0;
	virtual ClientInfo_t getClientAtIndex(DWORD index) = 0;

	virtual DWORD dwPitchBase() const = 0;
	virtual DWORD dwYawBase() const = 0;
	virtual DWORD dwLocalPlayerBase() const = 0;
};
