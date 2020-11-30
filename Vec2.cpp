#include "Vec2.h"
#include <cmath>

Vec2 Vec2::sf(const sf::Vector2f v)
{
	return { static_cast<double>(v.x), static_cast<double>(v.y) };
}

Vec2 Vec2::sf(const sf::Vector2i v)
{
	return { static_cast<double>(v.x), static_cast<double>(v.y) };
}

Vec2 Vec2::sf(const sf::Vector2u v)
{
	return { static_cast<double>(v.x), static_cast<double>(v.y) };
}

Vec2 Vec2::prim(unsigned int x, unsigned int y)
{
	return { static_cast<double>(x), static_cast<double>(y) };
}

Vec2 Vec2::prim(int x, int y)
{
	return { static_cast<double>(x), static_cast<double>(y) };
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

Vec2 Vec2::operator+(const Vec2 & other) const
{
	return { this->x + other.x, this->y + other.y };
}

Vec2 Vec2::operator-(const Vec2 & other) const
{
	return { this->x - other.x, this->y - other.y };
}

bool Vec2::operator==(const Vec2 & other) const
{
	return (x == other.x && y == other.y);
}
