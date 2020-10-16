#pragma once
/** 
represents a line that is part of a line fractal.
 - Line position is relative to an origin.
 - distances and angles are a proportion of a given length. (where 0.5 is half the length, 1 == the length and 2 is twice the length)
double distance the distance from the origin to the first point
double angle1 the angle in radians from the origin to the first point
double length the length of the line
double angle2 the angle in radians of the line
bool recursing true if the line is recursing
**/
struct LFLine {
	double distance;
	double angle1;
	double length;
	double angle2;
	bool recursing;
};