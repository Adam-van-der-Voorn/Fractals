#define _USE_MATH_DEFINES
#include <cmath>
#include "vecutil.h"
#include "KochSnowflake.h"
#include <memory>

#ifdef _DEBUG
#  include <iostream>
#  define PRINT(x) std::cout << x << std::endl;
#else
#  define PRINT(x) while(false){}
#endif

void KochSnowflake::recurse(const sf::Vector2f end_point, int limit)
{
	if (limit == 0) {
		relative_line.push_back(end_point);
	}
	else {
		const sf::Vector2f origin_point = relative_line[relative_line.size() - 1].position;
		const float line_angle = angle_between_ab(origin_point, end_point), subline_length = vec_len(end_point - origin_point) / 3;

		const sf::Vector2f first_third = origin_point + vec_from_len_angle(subline_length, line_angle),
			triangle_tip = first_third + vec_from_len_angle(subline_length, line_angle - (M_PI / 3)),
			second_third = first_third + vec_from_len_angle(subline_length, line_angle);

		recurse(first_third, limit - 1);
		recurse(triangle_tip, limit - 1);
		recurse(second_third, limit - 1);
		recurse(end_point, limit - 1);
	}
}

void KochSnowflake::generate(int recursions)
{
	relative_line.clear();
	relative_line.push_back(pa);
	recurse(pb, recursions);
	line.resize(relative_line.size());
	transfromLine();
}

const sf::VertexArray* KochSnowflake::getLine() const {
	return &line;
}

void KochSnowflake::transfromLine() {
	for (size_t i = 0; i < relative_line.size(); i++) {
		line[i] = relative_line[i];
		line[i].position.x *= scale;
		line[i].position.y *= scale;
		line[i].position += origin;
	}

}

void KochSnowflake::setPointA(sf::Vector2f pa)
{
	//this->pa = pa;
}

sf::Vector2f KochSnowflake::getPointA() const
{
	return pa;
}

void KochSnowflake::setPointB(sf::Vector2f pb)
{
	//this->pb = pb;
}

sf::Vector2f KochSnowflake::getPointB() const
{
	return pb;
}

void KochSnowflake::setScale(float new_scale, sf::Vector2f scale_point)
{
	scaleMult(new_scale / scale, scale_point);
}

float KochSnowflake::getScale() const
{
	return scale;
}

void KochSnowflake::scaleMult(float scale_multi, sf::Vector2f scale_point)
{
	sf::Vector2f diff = origin - scale_point;
	origin.x += diff.x * (scale_multi-1);
	origin.y += diff.y * (scale_multi-1);
	scale *= scale_multi;
	PRINT("zoom factor: " << getScale());
	PRINT("minimum float value * scale: " << FLT_EPSILON * getScale());
	PRINT("minimum double value * scale: " << DBL_EPSILON * getScale());
	transfromLine();
}

void KochSnowflake::setOrigin(sf::Vector2f new_origin)
{
	origin = new_origin;
	transfromLine();
}

void KochSnowflake::translate(sf::Vector2f translation)
{
	origin += translation;
	transfromLine();
}

sf::Vector2f KochSnowflake::getOrigin() const
{
	return origin;
}
