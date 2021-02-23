#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <ostream>
#include "pi.h"
struct Vec2 {
	double x;
	double y;

	Vec2(const sf::Vector2f v);
	Vec2(const sf::Vector2i v);
	Vec2(const sf::Vector2u v);
	Vec2(unsigned int x, unsigned int y);
	Vec2(int x, int y);
	Vec2(double x, double y);
	Vec2();
	static Vec2 fromLenDir(double len, double dir);

	void operator += (const Vec2& other);
	void operator -= (const Vec2& other);

	double angle() const;

	sf::Transform toSFTransform() const;
};

double dot(Vec2 a, Vec2 b);
double cross(const Vec2& a, const Vec2& b);
double dist(const Vec2& a, const Vec2& b);
inline double angleAtoB(const Vec2& a, const Vec2& b) {
	float line_angle = atanf((a.y - b.y) / (a.x - b.x));
	if (a.x >= b.x) {
		line_angle += m_pi;
	}
	return line_angle;
}
Vec2 operator + (const Vec2& a, const Vec2& b);
Vec2 operator - (const Vec2& a, const Vec2& b);
Vec2 operator / (const Vec2& vec, double scalar);
Vec2 operator * (const Vec2& vec, double scalar);
bool operator == (const Vec2& a, const Vec2& b);
bool operator != (const Vec2& a, const Vec2& b);
std::ostream& operator << (std::ostream& os, const Vec2& vec);
