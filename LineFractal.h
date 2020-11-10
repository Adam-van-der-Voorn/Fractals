#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>
#include "LFLine.h"
#include "AbsLine.h"

class LineFractal
{
public:
	LineFractal(AbsLine base_line, double scale = 1, double origin_x = 0, double origin_y = 0);

	// getters & setters
	void setScale(double new_scale, double scale_point_x, double scale_point_y);
	double getScale() const;
	void setOrigin(double x, double y);
	double getOriginX() const;
	double getOriginY() const;
	void setDerivedLines(std::vector<LFLine>& lines);
	void setBaseLine(AbsLine line);
		
	void zoom(double zoom_multi, double zoom_point_x, double zoom_point_y);
	void translate(double translation_x, double translation_y);

	void generate(int recursions);

	sf::VertexArray& getFractal();
	const std::vector<AbsLine>& getLines() const;
private:
	void recurse(AbsLine line, int limit);
	void transfromLine();

	double scale, origin_x, origin_y;
	AbsLine base_line;
	std::vector<LFLine> derived_lines;
	std::vector<AbsLine> lines;
	sf::VertexArray final_lines = sf::VertexArray(sf::Lines);



};


