#pragma once
#include "IOffset.h"
#include "Process.h"
#include "../Resources/StructsAndResources.h"

#define PITCH_OFFSET 0x467474
#define YAW_OFFSET PITCH_OFFSET + 4


class Tf2OffsetV2 : public IOffset
{
public:
	Tf2OffsetV2(Process* proc);

	~Tf2OffsetV2() override
	{
	};

	void printVerbose() override;
	// PlayerInfo_t getPlayerInfo(DWORD playerBase) override;
	PlayerInfo_t getLocalPlayerInfo() override;

	DWORD entityListBase() const override { return entityListBase_; };

	DWORD pitchBase() const override { return (DWORD)pitchBase_; };
	DWORD yawBase() const override { return (DWORD)yawBase_; };
	DWORD localPlayerBase() const override { return localPlayerBase_; };
	DWORD engineBase() const override { return engineBase_; };

	// ClientInfo_t getClientAtIndex(DWORD index) override;


	void testPrintHealths();

private:
	Process* procManager_ = 0;

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
