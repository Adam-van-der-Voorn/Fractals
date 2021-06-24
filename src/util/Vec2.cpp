#include "Vec2.h"
#include <cmath>



Vec2::Vec2(unsigned int x, unsigned int y) : x(static_cast<double>(x)), y(static_cast<double>(y))
{
}

Vec2::Vec2(int x, int y) : x(static_cast<double>(x)), y(static_cast<double>(y))
{
}

Vec2::Vec2(double x, double y) : x(x), y(y)
{
}

Vec2::Vec2()
{
	x = 0; y = 0;
}

Vec2 Vec2::fromLenDir(double len, double dir)
{
	return { cos(dir) * len, sin(dir) * len };
}

void Vec2::operator+=(const Vec2 & other)
{
	x += other.x;
	y += other.y;
}

void Vec2::operator-=(const Vec2 & other)
{
	x -= other.x;
	y -= other.y;
}

Vec2 Vec2::operator-()
{
	return Vec2(-x, -y);
}

double Vec2::angle() const
{
	return atan2(y, x);
}

double dot(Vec2 a, Vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

double cross(const Vec2 & a, const Vec2 & b)
{
	return (a.x * b.y) - (a.y * b.x);
}

double dist(const Vec2 & a, const Vec2 & b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double angleAtoB(const Vec2& a, const Vec2& b)
{
	double line_angle = atanf((a.y - b.y) / (a.x - b.x));
	if (a.x >= b.x) {
		//line_angle += m_pi;
	}
	return line_angle;
}

Vec2 operator+(const Vec2 & a, const Vec2 & b)
{
	return { a.x + b.x, a.y + b.y };
}

Vec2 operator-(const Vec2 & a, const Vec2 & b)
{
	return { a.x - b.x, a.y - b.y };
}

Vec2 operator-(const Vec2& a)
{
	return { Vec2(0,0)-a };
}

Vec2 operator/(const Vec2 & vec, double scalar)
{
	return { vec.x / scalar, vec.y / scalar };
}

Vec2 operator*(const Vec2 & vec, double scalar)
{
	return { vec.x * scalar, vec.y * scalar };
}

bool operator==(const Vec2 & a, const Vec2 & b)
{
	return (a.x == b.x && a.y == b.y);
}

bool operator!=(const Vec2 & a, const Vec2 & b)
{
	return !(a == b);
}

std::ostream & operator<<(std::ostream & os, const Vec2 & vec)
{
	os << '(' << vec.x << ", " << vec.y << ')';
	return os;
}
