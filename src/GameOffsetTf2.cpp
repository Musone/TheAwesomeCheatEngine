#include "GameOffsetTf2.h"

void GameOffsetTf2::init()
{
	// todo: initialize procMem
	throw "procMem stub";
	// procMem = 0; 
	dwClient_ = 0;
	dwClientSize_ = 0;
	dwEngine_ = 0;
	dwEngineSize_ = 0;
	dwLocalPlayer_ = 0;
	dwEntityList_ = 0;
	dwButtonBase_ = 0;
	dwButtonBase2_ = 0;
	dwDuck_ = 0;
	dwMoveForwards_ = 0;
	dwMoveBackwards_ = 0;
	dwJump_ = 0;
	dwAttack_ = 0;
	dwAttack2_ = 0;
	dwMoveLeft_ = 0;
	dwMoveRight_ = 0;
	dwLeft_ = 0;
	dwRight_ = 0;
	dwGetMaxClients_ = 0;
	dwIsInGame_ = 0;
	dwIsConnected_ = 0;
	dwGlowObjectManager_ = 0;
	dwWorldToScreen_ = 0;
	dwViewAngles_ = 0;
}

GameOffsetTf2::GameOffsetTf2() { init(); }

GameOffsetTf2::~GameOffsetTf2() { delete[] procMem; }

void GameOffsetTf2::getOffsets() { throw "stub"; }


void save() {
  std::ofstream saveF;

  saveF.open("tf2_offsets.txt");  // Defines the file

  time_t theTime = time(NULL);
  struct tm* aTime = localtime(&theTime);

  int day = aTime->tm_mday;
  int month = aTime->tm_mon + 1;
  int year = aTime->tm_year + 1900;
  int hour = aTime->tm_hour;
  int minute = aTime->tm_min;
  int second = aTime->tm_sec;

  // Formatting things

  saveF << "[TF2 Offset Dump | ";
  saveF << day;
  saveF << "/";
  saveF << month;
  saveF << "/";
  saveF << year;
  saveF << " ";
  saveF << hour;
  saveF << ":";
  saveF << minute;
  saveF << ":";
  saveF << second;  // Not the ideal method but eh
  saveF << "] \n\n";

  // Formatting the offsets

  saveF << "dwLocalPlayer : 0x" << hex << uppercase << Offset.dwLocalPlayer
        << "\n";
  saveF << "dwEntityList : 0x" << hex << uppercase << Offset.dwEntityList
        << "\n\n";

  saveF << "dwDuck : 0x" << hex << uppercase << Offset.dwDuck << "\n";
  saveF << "dwLeft : 0x" << hex << uppercase << Offset.dwLeft << "\n";
  saveF << "dwRight : 0x" << hex << uppercase << Offset.dwRight << "\n\n";
  saveF << "dwMoveLeft : 0x" << hex << uppercase << Offset.dwMoveLeft << "\n";
  saveF << "dwMoveRight : 0x" << hex << uppercase << Offset.dwMoveRight << "\n";
  saveF << "dwMoveForward : 0x" << hex << uppercase << Offset.dwMoveForwards
        << "\n";
  saveF << "dwMoveBackwards : 0x" << hex << uppercase << Offset.dwMoveBackwards
        << "\n\n";

  saveF << "dwJump : 0x" << hex << uppercase << Offset.dwJump << "\n";
  saveF << "dwAttack : 0x" << hex << uppercase << Offset.dwAttack << "\n";
  saveF << "dwAttack2 : 0x" << hex << uppercase << Offset.dwAttack2 << "\n\n";

  saveF << "dwGetMaxClients : 0x" << hex << uppercase << Offset.dwGetMaxClients
        << "\n";
  saveF << "dwIsInGame : 0x" << hex << uppercase << Offset.dwIsInGame << "\n";
  saveF << "dwIsConnected : 0x" << hex << uppercase << Offset.dwIsConnected
        << "\n";
  saveF << "dwGlowObjectManager : 0x" << hex << uppercase
        << Offset.dwGlowObjectManager << "\n";

  saveF << "dwViewAngles : 0x" << hex << uppercase << Offset.dwViewAngles
        << "\n";

  // Closed the files connection

  saveF.close();
}