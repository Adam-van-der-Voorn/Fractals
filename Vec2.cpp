#include "Vec2.h"
#include <cmath>

Vec2::Vec2(const sf::Vector2f v)
{
	x = static_cast<double>(v.x);
	y = static_cast<double>(v.y);
}

Vec2::Vec2(const sf::Vector2i v)
{
	x = static_cast<double>(v.x);
	y = static_cast<double>(v.y);
}

Vec2::Vec2(const sf::Vector2u v)
{
	x = static_cast<double>(v.x);
	y = static_cast<double>(v.y);
}

Vec2::Vec2(unsigned int x, unsigned int y)
{
	this->x = static_cast<double>(x);
	this->y = static_cast<double>(y);
}

Vec2::Vec2(int x, int y)
{
	this->x = static_cast<double>(x);
	this->y = static_cast<double>(y);
}

Vec2::Vec2(double x, double y)
{
	this->x = x;
	this->y = y;
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

double Vec2::angle() const
{
	return atan2(y, x);
}

sf::Transform Vec2::toSFTransform() const
{
	sf::Transform tmp;
	tmp.translate(x, y);
	return tmp;
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

Vec2 operator+(const Vec2 & a, const Vec2 & b)
{
	return { a.x + b.x, a.y + b.y };
}

Vec2 operator-(const Vec2 & a, const Vec2 & b)
{
	return { a.x - b.x, a.y - b.y };
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
