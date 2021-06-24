#include "Circle.h"
#include "RightAngleRect.h"

Circle::Circle()
{
}

Circle::Circle(const Vec2& center, double radius) : center(center), radius(radius)
{
}

double Circle::getRadius() const
{
	return radius;
}

Vec2 Circle::getCenter() const
{
	return center;
}

RightAngleRect Circle::getBBox() const
{
	return RightAngleRect::fromSize({ center.x - radius, center.y - radius }, { radius * 2, radius * 2 });
}


bool operator== (const Circle& a, const Circle& b) {
	return (a.getCenter() == b.getCenter() && a.getRadius() == b.getRadius());
}
bool operator!= (const Circle& a, const Circle& b) {
	return !(a == b);
}
std::ostream& operator<<(std::ostream& os, const Circle& circle)
{
	os << "( radius: " << circle.getRadius() << ", center: " << circle.getCenter() << ')';
	return os;
}
