#pragma once

#include <ostream>
#include "pi.h"
struct Vec2 {
	double x;
	double y;

	Vec2(unsigned int x, unsigned int y);
	Vec2(int x, int y);
	Vec2(double x, double y);
	Vec2();
	static Vec2 fromLenDir(double len, double dir);

	void operator += (const Vec2& other);
	void operator -= (const Vec2& other);
	Vec2 operator - ();


	double angle() const;
};

inline Vec2 abs(const Vec2& v) {
	return { abs(v.x), abs(v.y) };
}
double dot(Vec2 a, Vec2 b);
double cross(const Vec2& a, const Vec2& b);
double dist(const Vec2& a, const Vec2& b);
double angleAtoB(const Vec2& a, const Vec2& b);
Vec2 operator + (const Vec2& a, const Vec2& b);
Vec2 operator - (const Vec2& a, const Vec2& b);
Vec2 operator / (const Vec2& vec, double scalar);
Vec2 operator * (const Vec2& vec, double scalar);
bool operator == (const Vec2& a, const Vec2& b);
bool operator != (const Vec2& a, const Vec2& b);
std::ostream& operator << (std::ostream& os, const Vec2& vec);
