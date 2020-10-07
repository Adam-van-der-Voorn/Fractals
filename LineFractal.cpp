#define _USE_MATH_DEFINES
#include <cmath>
#include "vecutil.h"
#include <memory>

#ifdef _DEBUG
#  include <iostream>
#  define PRINT(x) std::cout << x << std::endl;
#else
#  define PRINT(x) while(false){}
#endif

#include "LineFractal.h"

LineFractal::LineFractal(double x1, double y1, double x2, double y2, float scale, sf::Vector2f origin) :
	pa(sf::Vector2f(x1, y1)), pb(sf::Vector2f(x2, y2)), scale(scale), origin(origin) 
{

	derived_lines = {
		{0, 0, 0.5, 0, false},
		{0.5, 0, 0.5, M_PI / 4, true},
		{0.5, 0, 0.5, -M_PI / 4, true}
	};
}

void LineFractal::recurse(sf::Vector2f start_point, sf::Vector2f end_point, int limit)
{
	if (limit == 0) {
		base_lines.push_back(start_point);
		base_lines.push_back(end_point);
	}
	else {
		const float line_angle = angle_between_ab(start_point, end_point), line_length = vec_len(end_point - start_point);
		for (int i = 0; i < derived_lines.size(); i++) {
			sf::Vector2f p1 = start_point + vec_from_len_angle(line_length * derived_lines[i].distance1, line_angle + derived_lines[i].angle1);
			sf::Vector2f p2 = p1 + vec_from_len_angle(line_length * derived_lines[i].distance2, line_angle + derived_lines[i].angle2);
			if (derived_lines[i].recursing) {
				recurse(p1, p2, limit - 1);
			}
			else {
				base_lines.push_back(p1);
				base_lines.push_back(p2);
			}
		}
	}
}

void LineFractal::generate(int recursions)
{
	base_lines.clear();
	recurse(pa, pb, recursions);
	final_lines.resize(base_lines.size()); // resize array length, not scaling
	transfromLine();
}

const sf::VertexArray* LineFractal::getFractal() const {
	return &final_lines;
}

void LineFractal::transfromLine() {
	for (size_t i = 0; i < base_lines.size(); i++) {
		final_lines[i] = base_lines[i];
		final_lines[i].position.x *= scale;
		final_lines[i].position.y *= scale;
		final_lines[i].position += origin;
	}
}

void LineFractal::setScale(float new_scale, sf::Vector2f scale_point)
{
	zoom(new_scale / scale, scale_point);
}

float LineFractal::getScale() const
{
	return scale;
}

void LineFractal::zoom(float zoom_multi, sf::Vector2f zoom_point)
{
	sf::Vector2f diff = origin - zoom_point;
	origin.x += diff.x * (zoom_multi - 1);
	origin.y += diff.y * (zoom_multi - 1);
	scale *= zoom_multi;
	PRINT("zoom factor: " << getScale());
	PRINT("minimum float value * scale: " << FLT_EPSILON * getScale());
	PRINT("minimum double value * scale: " << DBL_EPSILON * getScale());
	transfromLine();
}

void LineFractal::setOrigin(sf::Vector2f new_origin)
{
	origin = new_origin;
	transfromLine();
}

void LineFractal::translate(sf::Vector2f translation)
{
	origin += translation;
	transfromLine();
}

sf::Vector2f LineFractal::getOrigin() const
{
	return origin;
}
