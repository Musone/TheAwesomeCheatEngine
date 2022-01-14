#include "Geometry.h"


Angle_t Geometry::getRelativeAngle(Vec3_t player, LocalCoords_t local)
{
	Angle_t result = {0};
	Angle_t abs = getAbsoluteAngle(player, local);

	result.pitch = abs.pitch - local.pitch;
	result.yaw = abs.yaw - local.yaw;

	// if delta yaw is between 360 and 180 degrees, rotate in the other direction
	result.yaw +=
		360.f * (result.yaw <= -180.f) + -360.f * (result.yaw >= 180.f);

	return result;
}

Angle_t Geometry::getAbsoluteAngle(Vec3_t player, LocalCoords_t local)
{
	Angle_t result = {0};
	float xySlope, rzSlope;
	// float pitch, yaw;
	float dx, dy, dz, dr;

	dx = player.x - local.x;
	dy = player.y - local.y;
	dz = player.z - local.z;
	dr = sqrt(dy * dy + dx * dx);

	// adjustment for division by 0 error
	dx += 0.00001f * (dx == 0.f);
	dr += 0.00001f * (dr == 0.f);

	xySlope = dy / dx;
	rzSlope = dz / dr;

	result.pitch = (-1.0f) * atan(rzSlope) * 180.f / PI;
	result.yaw = atan(xySlope) * 180.f / PI;

	result.yaw += 180.f * (dx < 0); // Normalize angle

	// Contrict to bounds -180 < yaw < 180, -89 < pitch < 89
	result.pitch +=
		((-1.f) * result.pitch + 89.f) * (result.pitch > 89.f) +
		((-1.f) * result.pitch + -89.f) * (result.pitch < -89.f);
	result.yaw +=
		360.f * (result.yaw <= -180.f) + -360.f * (result.yaw >= 180.f);

	// if (result.yaw >= 180.f || result.pitch >= 90.f ||
	// 	result.yaw <= -180.f || result.pitch <= -90.f)
	// {
	// 	throw "CLAMP ERROR";
	// }

	return result;
}


Vec2_t Geometry::worldToScreen(Vec3_t pos, float* viewMatrix, Screen* screen)
{
	DWORD viewMatrixAddr = 0x7B59B7C8; // in engine:
	Vec2_t result = {-1};
	Vec4_t transform = {0};

	transform.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] +
		pos.z * viewMatrix[2] + viewMatrix[3];

	transform.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] +
		pos.z * viewMatrix[6] + viewMatrix[7];

	transform.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] +
		pos.z * viewMatrix[10] + viewMatrix[11];

	transform.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] +
		pos.z * viewMatrix[14] + viewMatrix[15];

	if (transform.w < 0.3f) return result;

	transform.x = transform.x / transform.w;
	transform.y = transform.y / transform.w;
	transform.z = transform.z / transform.w;

	result.x =
		(screen->width_ / 2.f * transform.x) +
		(transform.x + screen->width_ / 2.f);

	result.y =
		-(screen->height_ / 2.f * transform.y) +
		(transform.y + screen->height_ / 2.f);

	return result;
}

Angle_t Geometry::normalizeAngle(Angle_t angle, Screen* screen)
{
	angle.pitch = angle.pitch / (screen->yfov_ / 2.0);
	angle.yaw = angle.yaw / (screen->xfov_ / 2.0);
	return angle;
}

float Geometry::calculateDistance(Vec3_t from, Vec3_t to)
{
	Vec3_t d = {0};

	d.x = to.x - from.x;
	d.y = to.y - from.y;
	d.z = to.z - from.z;
	d.x += 0.0001f * (d.x == 0);
	float dr = sqrt(d.x * d.x + d.y * d.y);
	return sqrt(dr * dr + d.z * d.z);
}
