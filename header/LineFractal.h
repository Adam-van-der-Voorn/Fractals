#pragma once
#include <vector>
#include "LFLine.h"
#include "AbsLine.h"
#include "RightAngleRect.h"

class Circle;

class LineFractal
{


public:

	LineFractal(AbsLine base_line);

	// getters & setters
	void setDerivedLines(std::vector<LFLine>& lines);
	const std::vector<LFLine>& getDerivedLines() const;
	void setBaseLine(AbsLine line);
	AbsLine getBaseLine() const;
	void setView(RightAngleRect const& new_view);
	const RightAngleRect& getView() const;
		
	void translate(const Vec2& translation);

	bool generateIter(int steps, std::vector<AbsLine>& target) const;
	bool generateIter(int steps);
	void generate();
	void updateBounds();
	RightAngleRect getBoundsInstance(AbsLine line) const;
	const std::vector<AbsLine>& getLines() const;

	void setDefinition(double definition);
	double getDefinition() const;

private:
	static constexpr double ATOMIC_LENGTH = 4;

	// square that can be constructed from a given line
	// any recursions from the given line will be inside therse bounds
	struct Bounds {
		// the distance from the back of the line to the center of the bounds,
		// relaive to how long the line is.
		// e.g. if the distance is half as long as the line, offset = 0.5
		double offset; 

		// the difference between the angle of the line and the angle of the bounds offset
		double offset_angle; 

		// the shortest distance from the center to any edge of the bounds,
		// relaive to how long the line is.
		// e.g. if the radius is half as long as the line, radius = 0.5
		double radius; 
	};
	
	double definition = 1;
	Bounds bounds;
	RightAngleRect view;
	AbsLine base_line;
	std::vector<LFLine> derived_lines;
	std::vector<AbsLine> lines;
};


