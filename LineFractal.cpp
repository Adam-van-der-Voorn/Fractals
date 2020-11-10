//#include <cmath>
#include "pi.h"
#include "vecutil.h"
#include "LFLine.h"
#include "AbsLine.h"
#include <memory>
#include "debug_printing.h"
#include "LineFractal.h"

LineFractal::LineFractal(AbsLine base_line, double scale, double origin_x, double origin_y) :
	base_line(base_line), scale(scale), origin_x(origin_x), origin_y(origin_y)
{
	derived_lines = {
		{0, 0, 0.5, 0, true},
		{0.5, 0, 0.5, m_pi4, true},
		{0.5, 0, 0.5, -m_pi4, true}
	};
}

void LineFractal::recurse(AbsLine line, int limit)
{
	if (limit == 0) {

		lines.push_back(line);
	}
	else {
		double line_angle = lineAngle(line), line_length = lineLength(line);
		for (size_t i = 0; i < derived_lines.size(); i++) {
			// the distace from the start point of the parent line to the start point of this child line
			double a1 = line_length * derived_lines[i].distance; 
			// the angle from the start point of the parent line to the start point of this child line
			double a2 = line_angle + derived_lines[i].angle1;
			// the length of the child line
			double b1 = line_length * derived_lines[i].length;
			// the angle of the child line
			double b2 = line_angle + derived_lines[i].angle2;

			AbsLine new_line;
			new_line.x1 = line.x1 + lendirX(a1, a2);
			new_line.y1 = line.y1 + lendirY(a1, a2);
			new_line.x2 = new_line.x1 + lendirX(b1, b2);
			new_line.y2 = new_line.y1 + lendirY(b1, b2);
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
	final_lines.resize(lines.size()*2); // resize array length, not scaling
	transfromLine();
}

void LineFractal::setDerivedLines(std::vector<LFLine>& lines) {
	derived_lines = lines;
}

void LineFractal::setBaseLine(AbsLine line)
{
	base_line = line;
}

sf::VertexArray& LineFractal::getFractal() {
	return final_lines;
}

const std::vector<AbsLine>& LineFractal::getLines() const
{
	return lines;
}

void LineFractal::transfromLine() {
	int j = 0;
	for (size_t i = 0; i < final_lines.getVertexCount(); i+=2) {
		//float transformed_x = (float)((lines[j] * scale) + origin_x);
		//float transformed_y = (float)((lines[j + 1] * scale) + origin_y);
		final_lines[i] = sf::Vertex(sf::Vector2f(lines[j].x1, lines[j].y1));
		final_lines[i+1] = sf::Vertex(sf::Vector2f(lines[j].x2, lines[j].y2));
		j++;
	}
}

void LineFractal::setScale(double new_scale, double scale_point_x, double scale_point_y)
{
	/*zoom(new_scale / scale, scale_point_x, scale_point_y);*/
}

double LineFractal::getScale() const
{
	return scale;
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
	PRINT("zoom factor: " << getScale());
	PRINT("minimum float value * scale: " << FLT_EPSILON * getScale());
	PRINT("minimum double value * scale: " << DBL_EPSILON * getScale());
	transfromLine();*/
}

void LineFractal::translate(double translation_x, double translation_y)
{
	/*origin_x += translation_x;
	origin_y += translation_y;
	transfromLine();*/
}

double LineFractal::getOriginX() const
{
	return origin_x;
}

double LineFractal::getOriginY() const
{
	return origin_y;
}
