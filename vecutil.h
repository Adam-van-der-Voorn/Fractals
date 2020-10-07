#pragma once
#include "pi.h"
#include <cmath>
#include <SFML/System/Vector2.hpp>

inline float angle_between_ab(sf::Vector2f a, sf::Vector2f b) {
	float line_angle = atanf((a.y - b.y) / (a.x - b.x));
	if (a.x >= b.x) {
		line_angle += m_pi;
	}
	return line_angle;
}

inline double angle_between_ab(double ax, double ay, double bx, double by) {
	double line_angle = atanf((ay - by) / (ax - bx));
	if (ax >= bx) {
		line_angle += m_pi;
	}
	return line_angle;
}

inline float vec_len(sf::Vector2f v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

inline double vec_len(double vx, double vy) {
	return sqrt(pow(vx, 2) + pow(vy, 2));
}

/*
length in pixels
angle in radians
*/
inline sf::Vector2f vec_from_len_angle(float length, float angle) {
	float x = cos(angle) * length;
	float y = sin(angle) * length;
	return sf::Vector2f(x, y);
}

inline double lendir_x(double len, double dir) {
	return cos(dir) * len;
}


inline double lendir_y(double len, double dir) {
	return sin(dir) * len;
}