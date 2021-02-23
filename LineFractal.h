#pragma once
#include <vector>
#include "LFLine.h"
#include "AbsLine.h"

class LineFractal
{
public:

	LineFractal(AbsLine base_line);

	// getters & setters
	void setScale(double new_scale, double scale_point_x, double scale_point_y);
	double getScale() const;
	void setOrigin(double x, double y);
	double getOriginX() const;
	double getOriginY() const;
	void setDerivedLines(std::vector<LFLine>& lines);
	void setBaseLine(AbsLine line);
	void setView(double top, double bottom, double left, double right);
		
	void zoom(double zoom_multi, double zoom_point_x, double zoom_point_y);
	void translate(double translation_x, double translation_y);

	void generate(int recursions);
	bool generateIter(int steps, std::vector<AbsLine>& target) const;
	bool generateIter(int steps);
	void generate();
	void updateBounds();
	const std::vector<AbsLine>& getLines() const;

private:
	void recurse(AbsLine line, int limit);

	struct BoundingCircle {
		double offset;
		double offset_angle;
		double radius;
	};

	BoundingCircle bounds;
	double view_top, view_bottom, view_left, view_right;

	AbsLine base_line;
	std::vector<LFLine> derived_lines;
	std::vector<AbsLine> lines;
};


