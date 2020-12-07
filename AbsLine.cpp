#include "AbsLine.h"
#include <cmath>
#include "Vec2.h"

AbsLine AbsLine::fromLenAngle(Vec2 origin, double length, double angle)
{
	float x = cos(angle) * length;
	float y = sin(angle) * length;
	return { origin.x, origin.y, origin.x + x, origin.y + y };
}
