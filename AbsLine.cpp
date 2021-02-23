#include "AbsLine.h"
#include <cmath>
#include "Vec2.h"

AbsLine AbsLine::fromLenAngle(Vec2 origin, double length, double angle)
{
	float x = cos(angle) * length;
	float y = sin(angle) * length;
	return { origin.x, origin.y, origin.x + x, origin.y + y };
}

std::ostream & operator<<(std::ostream & os, const AbsLine & line)
{
	os << '(' << line.back_x << ", " << line.back_y << ")-->(" << line.head_x << ", " << line.head_y << ')';
	return os;
}
