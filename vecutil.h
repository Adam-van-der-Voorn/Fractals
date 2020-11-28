#pragma once
#include "pi.h"
#include "AbsLine.h"
#include <cmath>
#include <SFML/System/Vector2.hpp>

inline float angleBetweenAB(sf::Vector2f back_node, sf::Vector2f front_node) {
	float line_angle = atanf((back_node.y - front_node.y) / (back_node.x - front_node.x));
	if (back_node.x >= front_node.x) {
		line_angle += m_pi;
	}
	return line_angle;
}

inline double angleBetweenAB(double ax, double ay, double bx, double by) {
	double line_angle = atanf((ay - by) / (ax - bx));
	if (ax >= bx) {
		line_angle += m_pi;
	}
	return line_angle;
}

inline double lineAngle(AbsLine line) {
	double line_angle = atanf((line.back_y - line.head_y) / (line.back_x - line.head_x));
	if (line.back_x >= line.head_x) {
		line_angle += m_pi;
	}
	return line_angle;
}

inline float vecLen(sf::Vector2f v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

inline double distanceBetweenAB(double ax, double ay, double bx, double by) {
	return sqrt(pow(ax-bx, 2) + pow(ay-by, 2));
}

inline double lineLength(AbsLine line) {
	return sqrt(pow(line.back_x - line.head_x, 2) + pow(line.back_y - line.head_y, 2));
}

/*
length in pixels
angle in radians
*/
inline sf::Vector2f vecFromLenAngle(float length, float angle) {
	float x = cos(angle) * length;
	float y = sin(angle) * length;
	return sf::Vector2f(x, y);
}

inline double lendirX(double len, double dir) {
	return cos(dir) * len;
}


inline double lendirY(double len, double dir) {
	return sin(dir) * len;
}