#pragma once
# define _USE_MATH_DEFINES
# include <cmath>
# include <SFML/Graphics.hpp>
float angle_between_ab(sf::Vector2f a, sf::Vector2f b) {
	float line_angle = atanf((a.y - b.y) / (a.x - b.x));
	if (a.x >= b.x) {
		line_angle += M_PI;
	}
	return line_angle;
}

float vec_len(sf::Vector2f v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

/*
length in pixels
angle in radians
*/
sf::Vector2f vec_from_len_angle(float length, float angle) {
	float x = cos(angle) * length;
	float y = sin(angle) * length;
	return sf::Vector2f(x, y);
}