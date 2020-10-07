#pragma once
#include "Drawable.h"

class EditableLine : public Drawable
{
public:
	EditableLine(int id, double x1, double y1, double x2, double y2);
	bool attemptSelection(int x, int y);
	void translate(double dx, double dy);
	void setPosition(double x, double y);
	void setLength(double length);
	void setAngle(double angle);
	void switchSelection();

	void drawTo(sf::RenderTarget& surface) const;
private:
	int id;
	double x1, y1, x2, y2;
	enum Selection {
		START, END, NONE
	};
	Selection selected = NONE;
	const float nodeSize = 10;


};

