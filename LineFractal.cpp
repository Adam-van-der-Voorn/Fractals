//#include <cmath>
#include "pi.h"
#include "vecutil.h"
#include "LFLine.h"
#include "AbsLine.h"
#include "Vec2.h"
#include <memory>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <cassert>
#include <float.h>
#include "debug_printing.h"
#include "LineFractal.h"
#include "signum.h"
#include "convex_hull.h"

LineFractal::LineFractal(AbsLine base_line) :
	base_line(base_line)
{
	derived_lines = {
		{0, 0, 0.5, 0, true},
		{0.5, 0, 0.5, m_pi4, true},
		{0.5, 0, 0.5, -m_pi4, true}
	};
}

void LineFractal::recurse(AbsLine line, int limit)
{
	double line_length = lineLength(line);
	if (limit == 0 || line_length < 2) {
		lines.push_back(line);
	}
	else {
		double line_angle = lineAngle(line);
		for (size_t i = 0; i < derived_lines.size(); i++) {
			// the length of the child line
			double b1 = line_length * derived_lines[i].length;
			// the distace from the start point of the parent line to the start point of this child line
			double a1 = line_length * derived_lines[i].distance; 
			// the angle from the start point of the parent line to the start point of this child line
			double a2 = line_angle + derived_lines[i].angle1;
			// the angle of the child line
			double b2 = line_angle + derived_lines[i].angle2;

			AbsLine new_line;
			new_line.back_x = line.back_x + lendirX(a1, a2);
			new_line.back_y = line.back_y + lendirY(a1, a2);
			new_line.head_x = new_line.back_x + lendirX(b1, b2);
			new_line.head_y = new_line.back_y + lendirY(b1, b2);
			if (derived_lines[i].recursing) {
				recurse(new_line, limit - 1);
			}
			else {
				lines.push_back(new_line);
			}
		}
	}
}

void LineFractal::generate(int recursions)
{
	lines.clear();
	recurse(base_line, recursions);
}

// 1 step = 100 lines
bool LineFractal::generateIter(int steps, std::vector<AbsLine>& target) const
{
	target.clear();
	steps *= 100;
	std::queue<AbsLine> line_queue;
	line_queue.push(base_line);
	while (!line_queue.empty() && steps > 0) {
		AbsLine current_line = line_queue.front();
		line_queue.pop();
		double line_length = lineLength(current_line);
		if (line_length < 2) {
			target.push_back(current_line);
		}
		else {
			double line_angle = lineAngle(current_line);
			for (size_t i = 0; i < derived_lines.size(); i++) {
				// the length of the child line
				double b1 = line_length * derived_lines[i].length;
				// the distace from the start point of the parent line to the start point of this child line
				double a1 = line_length * derived_lines[i].distance;
				// the angle from the start point of the parent line to the start point of this child line
				double a2 = line_angle + derived_lines[i].angle1;
				// the angle of the child line
				double b2 = line_angle + derived_lines[i].angle2;

				AbsLine new_line;
				new_line.back_x = current_line.back_x + lendirX(a1, a2);
				new_line.back_y = current_line.back_y + lendirY(a1, a2);
				new_line.head_x = new_line.back_x + lendirX(b1, b2);
				new_line.head_y = new_line.back_y + lendirY(b1, b2);
				if (derived_lines[i].recursing) {
					line_queue.push(new_line);
				}
				else {
					target.push_back(new_line);
				}
			}
		}
		steps--;
	}
	if (steps <= 0) {
		return false;
	}
	return true;
}

bool LineFractal::generateIter(int steps)
{
	return generateIter(steps, lines);
}

void LineFractal::generate()
{
	lines.clear();
	std::queue<AbsLine> line_queue;
	line_queue.push(base_line);
	while (!line_queue.empty()) {
		AbsLine current_line = line_queue.front();
		line_queue.pop();
		double line_length = lineLength(current_line);
		if (line_length < 2) {
			lines.push_back(current_line);
		}
		else {
			double line_angle = lineAngle(current_line);
			for (size_t i = 0; i < derived_lines.size(); i++) {
				// the length of the child line
				double b1 = line_length * derived_lines[i].length;
				// the distace from the start point of the parent line to the start point of this child line
				double a1 = line_length * derived_lines[i].distance;
				// the angle from the start point of the parent line to the start point of this child line
				double a2 = line_angle + derived_lines[i].angle1;
				// the angle of the child line
				double b2 = line_angle + derived_lines[i].angle2;

				AbsLine new_line;
				new_line.back_x = current_line.back_x + lendirX(a1, a2);
				new_line.back_y = current_line.back_y + lendirY(a1, a2);
				new_line.head_x = new_line.back_x + lendirX(b1, b2);
				new_line.head_y = new_line.back_y + lendirY(b1, b2);
				if (derived_lines[i].recursing) {
					line_queue.push(new_line);
				}
				else {
					lines.push_back(new_line);
				}
			}
		}
	}
}

void LineFractal::setDerivedLines(std::vector<LFLine>& lines) {
	if (DEBUG) {
		double base_line_len = lineLength(base_line);
		for (LFLine line : lines) {
			assert(line.length < base_line_len && "derived line length cannot be > base line length");
		}
	}
	derived_lines = lines;
}

void LineFractal::setBaseLine(AbsLine line)
{
	base_line = line;
}

void LineFractal::setView(double top, double bottom, double left, double right)
{
	view_top = top;
	view_bottom = bottom;
	view_left = left;
	view_right = right;
}

const std::vector<AbsLine>& LineFractal::getLines() const
{
	return lines;
}

void LineFractal::updateBounds()
{
	std::vector<AbsLine> fractal_lines;
	generateIter(10000, fractal_lines);
	auto points = (std::vector<Vec2>&) fractal_lines;
	std::vector<Vec2> hull;
	putConvexHull(points, hull);
	double largest_distance = DBL_MIN;
	Vec2 current_edges[2];
	for (int i = 0; i < hull.size(); i++) {
		for (int ii = 0; ii < hull.size(); ii++) {
			if (i != ii) {
				double new_distance = dist(hull.at(i), hull.at(ii));
				if (new_distance > largest_distance) {
					largest_distance = new_distance;
					current_edges[0] = hull.at(i);
					current_edges[1] = hull.at(ii);
				}
			}
		}
	}
	Vec2 bounds_center = current_edges[0] + Vec2::fromLenDir(largest_distance * 0.5, angleAtoB(current_edges[0], current_edges[1]));
	bounds.offset = dist({ base_line.back_x, base_line.back_y }, bounds_center) / base_line.length();
	bounds.offset_angle = angleAtoB({ base_line.back_x, base_line.back_y }, bounds_center);
	bounds.radius = largest_distance;
}
	
void LineFractal::setScale(double new_scale, double scale_point_x, double scale_point_y)
{
	/*zoom(new_scale / scale, scale_point_x, scale_point_y);*/
}

void LineFractal::setOrigin(double x, double y)
{
	/*origin_x = x;
	origin_y = y;
	transfromLine();*/
}

void LineFractal::zoom(double zoom_multi, double zoom_point_x, double zoom_point_y)
{
	/*double diff_x = origin_x - zoom_point_x;
	double diff_y = origin_y - zoom_point_y;
	origin_x += diff_x * (zoom_multi - 1);
	origin_y += diff_y * (zoom_multi - 1);
	scale *= zoom_multi;
	PRINTLN("zoom factor: " << getScale());
	PRINTLN("minimum float value * scale: " << FLT_EPSILON * getScale());
	PRINTLN("minimum double value * scale: " << DBL_EPSILON * getScale());
	transfromLine();*/
}

void LineFractal::translate(double translation_x, double translation_y)
{
	/*origin_x += translation_x;
	origin_y += translation_y;
	transfromLine();*/
}