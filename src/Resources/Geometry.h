#pragma once

#include "StructsAndResources.h"
#include "Screen.h"
#include <math.h>

class Geometry
{
public:
	Angle_t getRelativeAngle(Vec3_t player, LocalCoords_t local);
	Angle_t getAbsoluteAngle(Vec3_t player, LocalCoords_t local);
	Vec2_t worldToScreen(Vec3_t pos, float* viewMatrix, Screen* screen);
	Angle_t normalizeAngle(Angle_t angle, Screen* screen);
	float calculateDistance(Vec3_t from, Vec3_t to);
};
