#pragma once

#include "Structs.h"

class IOffset
{
public:
	virtual ~IOffset()
	{
	};

	virtual void printVerbose() = 0;
	// virtual PlayerInfo_t getPlayerInfo(DWORD index) = 0;
	virtual PlayerInfo_t getLocalPlayerInfo() = 0;
	// virtual ClientInfo_t getClientAtIndex(DWORD index) = 0;

	virtual DWORD entityListBase() const = 0;
	virtual DWORD dwPitchBase() const = 0;
	virtual DWORD dwYawBase() const = 0;
	virtual DWORD dwLocalPlayerBase() const = 0;
};
