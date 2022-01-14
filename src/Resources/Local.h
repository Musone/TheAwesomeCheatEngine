#pragma once

#include "StructsAndResources.h"
#include "../Modules/Process.h"
#include "../Modules/IOffset.h"

class Local
{
public:
	// Vec3_t getTargetCoordinate();
	// Vec3_t getPlayerCoordinate(DWORD playerBase);
	// LocalCoords_t getLocalCoordinate();
	Process* process_ = 0;
	IOffset* offsets_ = 0;

	Local(Process* process, IOffset* offsets)
	{
		process_ = process;
		offsets_ = offsets;
	}

	LocalCoords_t getLocalCoordinate()
	{
		// todo: This shouldn't be static
		LocalCoords_t result = {0};
		process_->readAddress(offsets_->engineBase() + LOCAL_CAMERA_OFFSET,
		                      (BYTE*)&result, sizeof(result));
		return result;
	}
};
