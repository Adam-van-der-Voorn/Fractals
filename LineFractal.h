#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>
#include "RelLine.h"

class LineFractal
{
public:
	LineFractal(double x1, double y1, double x2, double y2, double scale = 1, double origin_x = 0, double origin_y = 0);

	// getters & setters
	void setScale(double new_scale, double scale_point_x, double scale_point_y);
	double getScale() const;
	void setOrigin(double x, double y);
	double getOriginX() const;
	double getOriginY() const;

	void zoom(double zoom_multi, double zoom_point_x, double zoom_point_y);
	void translate(double translation_x, double translation_y);

	void generate(int recursions);

	const sf::VertexArray* getFractal() const;
private:

	double x1, y1, x2, y2, scale, origin_x, origin_y;

	std::vector<double> base_lines;
	sf::VertexArray final_lines = sf::VertexArray(sf::Lines);

	void recurse(double x1, double y1, double x2, double y2, int limit);
	void transfromLine();

	std::vector<RelLine> derived_lines;
};


