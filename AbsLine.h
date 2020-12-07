#pragma once

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
};
