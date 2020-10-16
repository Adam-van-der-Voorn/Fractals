//#include <cmath>
#include "pi.h"
#include "vecutil.h"
#include "LFLine.h"
#include "AbsLine.h"
#include <memory>
#include "debug_printing.h"
#include "LineFractal.h"

LineFractal::LineFractal(double x1, double y1, double x2, double y2, double scale, double origin_x, double origin_y) :
	x1(x1), y1(y1), x2(x2), y2(y2), scale(scale), origin_x(origin_x), origin_y(origin_y)
{
	derived_lines = {
		{0, 0, 0.5, 0, true},
		{0.5, 0, 0.5, m_pi4, true},
		//{0.5, 0, 0.5, -m_pi4, true}
	};
}

void LineFractal::recurse(double x1, double y1, double x2, double y2, int limit)
{
	if (limit == 0) {
		base_lines.push_back(x1);
		base_lines.push_back(y1);
		base_lines.push_back(x2);
		base_lines.push_back(y2);
	}
	else {
		double line_angle = angleBetweenAB(x1, y1, x2, y2), line_length = distanceBetweenAB(x1, y1, x2, y2);
		for (size_t i = 0; i < derived_lines.size(); i++) {
			// the distace from the start point of the parent line to the start point of this child line
			double a1 = line_length * derived_lines[i].distance; 
			// the angle from the start point of the parent line to the start point of this child line
			double a2 = line_angle + derived_lines[i].angle1;
			// the length of the child line
			double b1 = line_length * derived_lines[i].length;
			// the angle of the child line
			double b2 = line_angle + derived_lines[i].angle2;

			double new_x1 = x1 + lendirX(a1, a2);
			double new_y1 = y1 + lendirY(a1, a2);
			double new_x2 = new_x1 + lendirX(b1, b2);
			double new_y2 = new_y1 + lendirY(b1, b2);
			if (derived_lines[i].recursing) {
				recurse(new_x1, new_y1, new_x2, new_y2, limit - 1);
			}
			else {
				base_lines.push_back(new_x1);
				base_lines.push_back(new_y1);
				base_lines.push_back(new_x2);
				base_lines.push_back(new_y2);
			}
		}
	}
}

void LineFractal::generate(int recursions)
{
	base_lines.clear();
	recurse(x1, y1, x2, y2, recursions);
	final_lines.resize(base_lines.size()/2); // resize array length, not scaling
	transfromLine();
}

void LineFractal::setDerivedLines(std::vector<LFLine>& lines) {
	derived_lines = lines;
}

void LineFractal::setBaseLine(AbsLine line)
{
	x1 = line.x1;
	x2 = line.x2;
	y1 = line.y1;
	y2 = line.y2;
}

sf::VertexArray& LineFractal::getFractal() {
	return final_lines;
}

void LineFractal::transfromLine() {
	int j = 0; // base lines index
	for (size_t i = 0; i < final_lines.getVertexCount(); i++) {
		float transformed_x = (float)((base_lines[j] * scale) + origin_x);
		float transformed_y = (float)((base_lines[j + 1] * scale) + origin_y);
		final_lines[i] = sf::Vertex(sf::Vector2f(transformed_x, transformed_y));
		j += 2;
	}
}

void LineFractal::setScale(double new_scale, double scale_point_x, double scale_point_y)
{
	zoom(new_scale / scale, scale_point_x, scale_point_y);
}

double LineFractal::getScale() const
{
	return scale;
}

void LineFractal::setOrigin(double x, double y)
{
	origin_x = x;
	origin_y = y;
	transfromLine();
}

void LineFractal::zoom(double zoom_multi, double zoom_point_x, double zoom_point_y)
{
	double diff_x = origin_x - zoom_point_x;
	double diff_y = origin_y - zoom_point_y;
	origin_x += diff_x * (zoom_multi - 1);
	origin_y += diff_y * (zoom_multi - 1);
	scale *= zoom_multi;
	PRINT("zoom factor: " << getScale());
	PRINT("minimum float value * scale: " << FLT_EPSILON * getScale());
	PRINT("minimum double value * scale: " << DBL_EPSILON * getScale());
	transfromLine();
}

void LineFractal::translate(double translation_x, double translation_y)
{
	origin_x += translation_x;
	origin_y += translation_y;
	transfromLine();
}

double LineFractal::getOriginX() const
{
	return origin_x;
}

double LineFractal::getOriginY() const
{
	return origin_y;
}
