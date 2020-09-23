
#define _USE_MATH_DEFINES
#include <cmath>
#include "vecutil.h"
#include "MultiLineSnowflake.h"
#include <memory>

#ifdef _DEBUG
#  include <iostream>
#  define PRINT(x) std::cout << x << std::endl;
#else
#  define PRINT(x) while(false){}
#endif

void MultiLineSnowflake::recurse(sf::Vector2f start_point, sf::Vector2f end_point, int limit)
{
	if (limit == 0) {
		relative_line.push_back(start_point);
		relative_line.push_back(end_point);
	}
	else {
		const float line_angle = angle_between_ab(start_point, end_point), subline_length = vec_len(end_point - start_point) / 3;

		sf::Vector2f a_start = start_point;
		sf::Vector2f a_end = start_point + vec_from_len_angle(subline_length, line_angle);

		sf::Vector2f b_start = a_end; //start_point + vec_from_len_angle(subline_length, line_angle); // a_end
		sf::Vector2f b_end = a_end + vec_from_len_angle(subline_length, line_angle - (M_PI / 3));

		sf::Vector2f c_start = b_end; // a_end + vec_from_len_angle(subline_length, line_angle - (M_PI / 3)); // b_end
		sf::Vector2f c_end = a_end + vec_from_len_angle(subline_length, line_angle);

		sf::Vector2f d_start = c_end; // a_end + vec_from_len_angle(subline_length, line_angle); // c_end
		sf::Vector2f d_end = c_end + vec_from_len_angle(subline_length, line_angle);
		recurse(a_start, a_end, limit - 1);
		recurse(b_start, b_end, limit - 1);
		recurse(c_start, c_end, limit - 1);
		recurse(d_start, d_end, limit - 1);
	}
}

void MultiLineSnowflake::generate(int recursions)
{
	relative_line.clear();
	recurse(pa, pb, recursions);
	line.resize(relative_line.size());
	transfromLine();
}

const sf::VertexArray* MultiLineSnowflake::getLine() const {
	return &line;
}

void MultiLineSnowflake::transfromLine() {
	for (size_t i = 0; i < relative_line.size(); i++) {
		line[i] = relative_line[i];
		line[i].position.x *= scale;
		line[i].position.y *= scale;
		line[i].position += origin;
	}

}

void MultiLineSnowflake::setPointA(sf::Vector2f pa)
{
	//this->pa = pa;
}

sf::Vector2f MultiLineSnowflake::getPointA() const
{
	return pa;
}

void MultiLineSnowflake::setPointB(sf::Vector2f pb)
{
	//this->pb = pb;
}

sf::Vector2f MultiLineSnowflake::getPointB() const
{
	return pb;
}

void MultiLineSnowflake::setScale(float new_scale, sf::Vector2f scale_point)
{
	scaleMult(new_scale / scale, scale_point);
}

float MultiLineSnowflake::getScale() const
{
	return scale;
}

void MultiLineSnowflake::scaleMult(float scale_multi, sf::Vector2f scale_point)
{
	sf::Vector2f diff = origin - scale_point;
	origin.x += diff.x * (scale_multi - 1);
	origin.y += diff.y * (scale_multi - 1);
	scale *= scale_multi;
	PRINT("zoom factor: " << getScale());
	PRINT("minimum float value * scale: " << FLT_EPSILON * getScale());
	PRINT("minimum double value * scale: " << DBL_EPSILON * getScale());
	transfromLine();
}

void MultiLineSnowflake::setOrigin(sf::Vector2f new_origin)
{
	origin = new_origin;
	transfromLine();
}

void MultiLineSnowflake::translate(sf::Vector2f translation)
{
	origin += translation;
	transfromLine();
}

sf::Vector2f MultiLineSnowflake::getOrigin() const
{
	return origin;
}
