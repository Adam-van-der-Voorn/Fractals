#include "AbsLine.h"
#include <cmath>
#include "Vec2.h"

AbsLine AbsLine::fromLenAngle(Vec2 origin, double length, double angle)
{
	Vec2 other = { cos(angle) * length, sin(angle) * length };
	return { origin, origin + other };
}

double AbsLine::length()
{
	return sqrt(pow(head.x - back.x, 2) + pow(head.y - back.y, 2));
}

double AbsLine::angle()
{
	double line_angle = atanf((back.y - head.y) / (back.x - head.x));
	if (back.x >= head.x) {
		line_angle += m_pi;
	}
	return line_angle;
}

std::ostream & operator<<(std::ostream & os, const AbsLine & line)
{
	os << '(' << line.back.x << ", " << line.back.y << ")-->(" << line.head.x << ", " << line.head.y << ')';
	return os;
}
