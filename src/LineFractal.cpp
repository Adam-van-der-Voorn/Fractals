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
#include "intersections.h"
#include "LineFractal.h"
#include "signum.h"
#include "Circle.h"
#include "intersections.h"
#include "smallest_enclosing_circle.h"

LineFractal::LineFractal(AbsLine base_line) :
	base_line(base_line)
{
	derived_lines = {};
}

// 1 step = 100 lines
std::vector<AbsLine> LineFractal::generateIter(int steps) const
{
	std::vector<AbsLine> lines = {};
	steps *= 100;
	double line_threshold = ATOMIC_LENGTH * getDefinition();
	std::queue<AbsLine> line_queue;
	line_queue.push(base_line);
	while (!line_queue.empty() && steps > 0) {
		AbsLine current_line = line_queue.front();
		line_queue.pop();
		double line_length = lineLength(current_line);
		if (line_length < line_threshold) {
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
				new_line.back.x = current_line.back.x + lendirX(a1, a2);
				new_line.back.y = current_line.back.y + lendirY(a1, a2);
				new_line.head.x = new_line.back.x + lendirX(b1, b2);
				new_line.head.y = new_line.back.y + lendirY(b1, b2);
				if (derived_lines[i].recursing) {
					line_queue.push(new_line);
				}
				else {
					lines.push_back(new_line);
				}
			}
		}
		steps--;
	}
	return lines;
}

RightAngleRect LineFractal::getBoundsInstance(AbsLine line) const
{
	double line_length = line.length();
	double radius = bounds.radius * line_length;
	Vec2 center = Vec2(line.back.x, line.back.y) + Vec2::fromLenDir(bounds.offset * line_length, bounds.offset_angle + line.angle());
	return RightAngleRect(center - Vec2(radius, radius), center + Vec2(radius, radius));
}

std::vector<AbsLine> LineFractal::generate() const
{
	std::vector<AbsLine> lines = {};
	std::queue<AbsLine> line_queue;
	line_queue.push(base_line);
	double line_threshold = ATOMIC_LENGTH * getDefinition();
	while (!line_queue.empty()) {
		AbsLine current_line = line_queue.front();
		line_queue.pop();
		double line_length = lineLength(current_line);
		if (line_length < line_threshold) {
			lines.push_back(current_line);
		}
		else {
			// check bounds
			RightAngleRect local_bounds = getBoundsInstance(current_line);
			if (isIntersectingIncl(local_bounds, view)) {
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
					new_line.back.x = current_line.back.x + lendirX(a1, a2);
					new_line.back.y = current_line.back.y + lendirY(a1, a2);
					new_line.head.x = new_line.back.x + lendirX(b1, b2);
					new_line.head.y = new_line.back.y + lendirY(b1, b2);
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
	return lines;
}

void LineFractal::setDerivedLines(std::vector<LFLine>& lines) {
	if (DEBUG) {
		double base_line_len = lineLength(base_line);
		for (const LFLine& line : lines) {
			assert(line.length < base_line_len && "derived line length cannot be > base line length");
		}
	}
	derived_lines = lines;
}

const std::vector<LFLine>& LineFractal::getDerivedLines() const
{
	return derived_lines;
}

void LineFractal::setBaseLine(AbsLine line)
{
	base_line = line;
}

AbsLine LineFractal::getBaseLine() const
{
	return base_line;
}

void LineFractal::setView(RightAngleRect const& new_view)
{
	view = new_view;
}

const RightAngleRect& LineFractal::getView() const
{
	return view;
}

const std::vector<AbsLine>& LineFractal::getLines() const
{
	return lines;
}

void LineFractal::setDefinition(double definition)
{
	this->definition = definition;
}

double LineFractal::getDefinition() const
{
	return definition;
}

void LineFractal::updateBounds()
{
	std::vector<AbsLine> fractal_lines = generateIter(10000);
	auto points = (std::vector<Vec2>&) fractal_lines;
	Circle abs_circle = makeSmallestEnclosingCircle(points);
	double base_line_length = base_line.length();
	bounds.radius = abs_circle.getRadius() / base_line_length;
	bounds.offset = dist({ base_line.back.x, base_line.back.y }, abs_circle.getCenter()) / base_line.length();
	bounds.offset_angle = angleAtoB({ base_line.back.x, base_line.back.y }, abs_circle.getCenter());
}

void LineFractal::translate(const Vec2& translation)
{
	base_line.back += translation;
	base_line.head += translation;
}