#pragma once
#include <ostream>
struct Vec2;

/**
a line represented by coords of its start and end points
double back_x x position of back
double back_y y position of back
double head_x x position of head
double head_y y position of head
**/
struct AbsLine {
	double back_x;
	double back_y;
	double head_x;
	double head_y;

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
	double length() {
		return sqrt(pow(back_x - head_x, 2) + pow(back_y - head_y, 2));
	}
};

std::ostream & operator<<(std::ostream & os, const AbsLine & line);

