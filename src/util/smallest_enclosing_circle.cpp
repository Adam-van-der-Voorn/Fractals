/*
 * Modified from:
 * Smallest enclosing circle - Library (C++)
 *
 * Copyright (c) 2020 Project Nayuki
 * https://www.nayuki.io/page/smallest-enclosing-circle
 */

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include "smallest_enclosing_circle.h"
#include "Circle.h"

using std::size_t;
using std::vector;
using std::max;
using std::min;


/*---- Members of struct Point ----*/

Point Point::subtract(const Point& p) const {
	return Point{ x - p.x, y - p.y };
}


double Point::distance(const Point& p) const {
	return std::hypot(x - p.x, y - p.y);
}


double Point::cross(const Point& p) const {
	return x * p.y - y * p.x;
}


/*---- Members of struct NayukiCircle ----*/

const NayukiCircle NayukiCircle::INVALID{ Point{0, 0}, -1 };

const double NayukiCircle::MULTIPLICATIVE_EPSILON = 1 + 1e-14;


bool NayukiCircle::contains(const Point& p) const {
	return c.distance(p) <= r * MULTIPLICATIVE_EPSILON;
}


bool NayukiCircle::contains(const vector<Point>& ps) const {
	for (const Point& p : ps) {
		if (!contains(p))
			return false;
	}
	return true;
}


/*---- Smallest enclosing circle algorithm ----*/

static NayukiCircle makeSmallestEnclosingCircleOnePoint(const vector<Point>& points, size_t end, const Point& p);
static NayukiCircle makeSmallestEnclosingCircleTwoPoints(const vector<Point>& points, size_t end, const Point& p, const Point& q);

static std::default_random_engine randGen((std::random_device())());

NayukiCircle makeDiameter(const Point& a, const Point& b) {
	Point c{ (a.x + b.x) / 2, (a.y + b.y) / 2 };
	return NayukiCircle{ c, max(c.distance(a), c.distance(b)) };
}

NayukiCircle makeCircumcircle(const Point& a, const Point& b, const Point& c);

// Initially: No boundary points known
Circle makeSmallestEnclosingCircle(const vector<Vec2>& points) {
	// Clone list to preserve the caller's data, randomize order
	auto points_converted = (vector<Point>&) points;
	vector<Point> shuffled = points_converted;
	std::shuffle(shuffled.begin(), shuffled.end(), randGen);

	// Progressively add points to circle or recompute circle
	NayukiCircle c = NayukiCircle::INVALID;
	for (size_t i = 0; i < shuffled.size(); i++) {
		const Point& p = shuffled.at(i);
		if (c.r < 0 || !c.contains(p))
			c = makeSmallestEnclosingCircleOnePoint(shuffled, i + 1, p);
	}
	return Circle({ c.c.x, c.c.y }, c.r);
}


// One boundary point known
static NayukiCircle makeSmallestEnclosingCircleOnePoint(const vector<Point>& points, size_t end, const Point& p) {
	NayukiCircle c{ p, 0 };
	for (size_t i = 0; i < end; i++) {
		const Point& q = points.at(i);
		if (!c.contains(q)) {
			if (c.r == 0)
				c = makeDiameter(p, q);
			else
				c = makeSmallestEnclosingCircleTwoPoints(points, i + 1, p, q);
		}
	}
	return c;
}


// Two boundary points known
static NayukiCircle makeSmallestEnclosingCircleTwoPoints(const vector<Point>& points, size_t end, const Point& p, const Point& q) {
	NayukiCircle circ = makeDiameter(p, q);
	NayukiCircle left = NayukiCircle::INVALID;
	NayukiCircle right = NayukiCircle::INVALID;

	// For each point not in the two-point circle
	Point pq = q.subtract(p);
	for (size_t i = 0; i < end; i++) {
		const Point& r = points.at(i);
		if (circ.contains(r))
			continue;

		// Form a circumcircle and classify it on left or right side
		double cross = pq.cross(r.subtract(p));
		NayukiCircle c = makeCircumcircle(p, q, r);
		if (c.r < 0)
			continue;
		else if (cross > 0 && (left.r < 0 || pq.cross(c.c.subtract(p)) > pq.cross(left.c.subtract(p))))
			left = c;
		else if (cross < 0 && (right.r < 0 || pq.cross(c.c.subtract(p)) < pq.cross(right.c.subtract(p))))
			right = c;
	}

	// Select which circle to return
	if (left.r < 0 && right.r < 0)
		return circ;
	else if (left.r < 0)
		return right;
	else if (right.r < 0)
		return left;
	else
		return left.r <= right.r ? left : right;
}


NayukiCircle makeCircumcircle(const Point& a, const Point& b, const Point& c) {
	// Mathematical algorithm from Wikipedia: Circumscribed circle
	double ox = (min(min(a.x, b.x), c.x) + max(max(a.x, b.x), c.x)) / 2;
	double oy = (min(min(a.y, b.y), c.y) + max(max(a.y, b.y), c.y)) / 2;
	double ax = a.x - ox, ay = a.y - oy;
	double bx = b.x - ox, by = b.y - oy;
	double cx = c.x - ox, cy = c.y - oy;
	double d = (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) * 2;
	if (d == 0)
		return NayukiCircle::INVALID;
	double x = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by)) / d;
	double y = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax)) / d;
	Point p{ ox + x, oy + y };
	double r = max(max(p.distance(a), p.distance(b)), p.distance(c));
	return NayukiCircle{ p, r };
}