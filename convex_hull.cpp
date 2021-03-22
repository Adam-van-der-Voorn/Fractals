#include "convex_hull.h"
#include "debug_printing.h"
#include <algorithm>

void putConvexHull(std::vector<Vec2> points, std::vector<Vec2>& target)
{
	if (points.size() == 0) {
		return;
	}
	// find lowest point 
	Vec2 lowest_point = { DBL_MIN, DBL_MIN };
	int lowest_point_index;
	for (size_t i = 0; i < points.size();) {
		if (points[i].y > lowest_point.y) {
			lowest_point = points[i];
			lowest_point_index = i;
			i++;
		}
		else if (points[i].x == lowest_point.x) { // duplicate lowest point
			points.erase(points.begin() + i);
		}
		else {
			i++;
		}
	}
	points.erase(points.begin() + lowest_point_index);
	std::sort(points.begin(), points.end(), [](const Vec2& a, const Vec2& b) { return a.y < b.y; });
	// sort points by angle off lowest point
	auto comp = CompareAngleFromPoint(lowest_point);
	std::sort(points.begin(), points.end(), comp);
	PRINTLN("old size : " << points.size());

	// remove dupe points and points with the same angle from the lowest point
	for (size_t i = 0; i < points.size() - 1;) {
		if (points[i] == points[i + 1]) {
			points.erase(points.begin() + i);
		}
		else {
			Vec2 current = points[i] - lowest_point, next = points[i + 1] - lowest_point;
			if (static_cast<float>(psudeoAngle(current)) == static_cast<float>(psudeoAngle(next))) {
				if ((abs(current.x) + abs(current.y)) < (abs(next.x) + abs(next.y))) {
					points.erase(points.begin() + i);
				}
				else {
					points.erase(points.begin() + i + 1);
				}
			}
			else {
				i++;
			}
		}
	}

	//PRINTLN("new size : " << points.size());
	for (const Vec2& vec : points) {
		/*PRINT(vec << "\t");
		PRINT("angle: " << atan2(vec.y - lowest_point.y, vec.x - lowest_point.x) << "\t");
		PRINTLN("pseud: " << psudeoAngle(vec - lowest_point));*/

	}

	// function to find out if the next point is a left or right turn;
	enum Turn { COUNTER_CLOCKWISE, CLOCKWISE, COVARIANT };
	auto turnDir = [](const std::vector<Vec2>& hull, const Vec2& new_point) -> Turn {
		size_t hull_size = hull.size();
		const Vec2 previous_line = hull.at(hull.size() - 1) - hull.at(hull.size() - 2);
		const Vec2 line_candidate = new_point - hull.at(hull.size() - 2);
		double turn = (cross(previous_line, line_candidate));
		if (turn < 0) return CLOCKWISE;
		else if (turn > 0) return COUNTER_CLOCKWISE;
		else return COVARIANT;
	};

	std::vector<Vec2>& hull = target;
	hull.push_back(lowest_point);
	hull.push_back(points[0]);
	for (int i = 1; i < points.size(); i++) {
		Turn turn = turnDir(hull, points[i]);
		if (turn == COUNTER_CLOCKWISE) {
			hull.push_back(points[i]);
		}
		else if (turn == CLOCKWISE) {
			hull.pop_back();
			Turn turn = turnDir(hull, points[i]);
			while (turn == CLOCKWISE) {
				hull.pop_back();
				turn = turnDir(hull, points[i]);
			}
			if (turn == COUNTER_CLOCKWISE) { 
				hull.push_back(points[i]);
			}
			else { // turn is covariant
				hull.pop_back();
				hull.push_back(points[i]);
			}
		}
		else if (turn == COVARIANT) {
			hull.pop_back();
			hull.push_back(points[i]);
		}
	}
}

CompareAngleFromPoint::CompareAngleFromPoint(const Vec2 & base_point) : base_point(base_point) {}

bool CompareAngleFromPoint::operator()(const Vec2 & a, const Vec2 & b) const
{
	Vec2 a_line = a - base_point;
	Vec2 b_line = b - base_point;
	float a_psued = static_cast<float>(psudeoAngle(a_line));
	float b_psued = static_cast<float>(psudeoAngle(b_line));
	return a_psued < b_psued;
}

