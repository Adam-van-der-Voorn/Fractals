#pragma 
#include <vector>
#include "Vec2.h"
#include <cassert>
void putConvexHull(std::vector<Vec2> points, std::vector<Vec2>& target);

double getConvexHullCircleBounds(const std::vector<Vec2>& hull);

class CompareAngleFromPoint final
{
private:
	const Vec2 base_point;
public:
	CompareAngleFromPoint(const Vec2& base_point);

	bool operator() (const Vec2& a, const Vec2& b) const;
};

/**
based off https://stackoverflow.com/questions/16542042/fastest-way-to-sort-vectors-by-angle-without-actually-computing-that-angle
\param v a vector with a y value <= 0. A vector of (0, 0) will also throw an exception.
\return a number from the range [-1 .. 1] which is monotonic in the angle this vector makes against the x axis.
**/
inline double psudeoAngle(const Vec2& v)
{
	assert(v != (Vec2{ 0, 0 }) && "invalid param where v = (0,0)");
	assert(v.y <= 0 && "invalid param where v.y > 0");
	Vec2 abs_v = { abs(v.x), abs(v.y) };
	double p = v.x / (abs_v.x + abs_v.y);
	return p;
}