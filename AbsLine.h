#pragma once
#include <ostream>
#include "Vec2.h"

/**
a line represented by coords of its start and end points
Vec2 back position of back
Vec2 head position of head
**/
struct AbsLine {
	Vec2 back;
	Vec2 head;

	/**
	Creates an absline with a given length and angle.
	The first point is given, and the second point is derived from the length and angle.
	\param origin the given point
	\param length the length of the line
	\param angle the angle of the line (in rads)
	**/
	static AbsLine fromLenAngle(Vec2 origin, double length, double angle);

	/**
	\return the length of this line
	**/
	double length();

	/**
	\return the angle of the line
	**/
	double angle();
	
};

std::ostream & operator<<(std::ostream & os, const AbsLine & line);

