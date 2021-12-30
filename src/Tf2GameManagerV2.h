#pragma once
#include "IGameManager.h"
#include "ProcessManager.h"

#define PITCH_OFFSET 0x467474
#define YAW_OFFSET PITCH_OFFSET + 4
#define MAX_PLAYERS 24

typedef struct Entity
{
	BYTE pad0[0xA8];
	DWORD hp;
	BYTE pad1[0x1E0];
	float x;
	float y;
	float z;
	BYTE pad2[0xE04];
	DWORD observermode;
	BYTE pad3[0x6DC];
	DWORD cursorid;
} Entity_t;

class Tf2GameManagerV2 : public IGameManager
{
public:
	Tf2GameManagerV2(ProcessManager* proc);

	~Tf2GameManagerV2() override
	{
	};

	void printVerbose() override;
	PlayerInfo_t getPlayerInfo(DWORD playerBase) override;
	PlayerInfo_t getLocalPlayerInfo() override; // todo: add this to the interface

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
};
