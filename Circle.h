#pragma once
#include "Vec2.h"

class RightAngleRect;

class Circle
{
public:
	Circle();
	Circle(const Vec2& center, double radius);
	double getRadius() const;
	Vec2 getCenter() const;
	RightAngleRect getBBox() const;
private:
	Vec2 center;
	double radius;
};

bool operator == (const Circle& a, const Circle& b);
bool operator != (const Circle& a, const Circle& b);
std::ostream& operator << (std::ostream& os, const Circle& vec);

