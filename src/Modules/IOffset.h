#pragma once

#include "../Resources/StructsAndResources.h"

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
	virtual DWORD pitchBase() const = 0;
	virtual DWORD yawBase() const = 0;
	virtual DWORD localPlayerBase() const = 0;
	virtual DWORD engineBase() const = 0;
};
