#include "intersections.h"
#include "RightAngleRect.h"
#include "Circle.h"
bool isIntersectingIncl(RightAngleRect rectangle_a, RightAngleRect rectangle_b)
{
	if (rectangle_a.top() <= rectangle_b.bottom() && rectangle_a.bottom() >= rectangle_b.top() &&
		rectangle_a.left() <= rectangle_b.right() && rectangle_a.right() >= rectangle_b.left()) {
		return true;
	}
	return false;
}

bool isIntersectingIncl(Circle circle, RightAngleRect rectangle)
{
	RightAngleRect circle_bbox = circle.getBBox();
	if (isIntersectingIncl(rectangle, circle_bbox)) {
		return true;
	}
	return false;
}

