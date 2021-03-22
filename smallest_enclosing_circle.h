/*
 * Modified version of:
 * Smallest enclosing circle - Library (C++)
 *
 * Copyright (c) 2017 Project Nayuki
 * https://www.nayuki.io/page/smallest-enclosing-circle
 */

#pragma once

#include <vector>

struct Vec2;
struct Circle;

struct Point final {

public: double x;
public: double y;


public: Point subtract(const Point& p) const;

public: double distance(const Point& p) const;

	  // Signed area / determinant thing
public: double cross(const Point& p) const;

};


struct NayukiCircle final {

public: static const NayukiCircle INVALID;

private: static const double MULTIPLICATIVE_EPSILON;


public: Point c;   // Center
public: double r;  // Radius


public: bool contains(const Point& p) const;

public: bool contains(const std::vector<Point>& ps) const;

};


/*
 * (Main function) Returns the smallest circle that encloses all the given points.
 * Runs in expected O(n) time, randomized. Note: If 0 points are given, a circle of
 * negative radius is returned. If 1 point is given, a circle of radius 0 is returned.
 */
Circle makeSmallestEnclosingCircle(const std::vector<Vec2>& points);

// For unit tests
//NayukiCircle makeDiameter(const Point& a, const Point& b);
//NayukiCircle makeCircumcircle(const Point& a, const Point& b, const Point& c);