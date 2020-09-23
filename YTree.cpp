#define _USE_MATH_DEFINES
#include <cmath>
#include "vecutil.h"
#include <memory>
#include "YTree.h"


#ifdef _DEBUG
#  include <iostream>
#  define PRINT(x) std::cout << x << std::endl;
#else
#  define PRINT(x) while(false){}
#endif

void YTree::recurse(sf::Vector2f start_point, sf::Vector2f end_point, int limit)
{
	if (limit == 0) {
		pretransform_lines.push_back(start_point);
		pretransform_lines.push_back(end_point);
	}
	else {
		const float line_angle = angle_between_ab(start_point, end_point), line_length = vec_len(end_point - start_point);

		sf::Vector2f a1 = start_point;
		sf::Vector2f a2 = start_point + vec_from_len_angle(line_length / 2, line_angle);

		sf::Vector2f b1 = a2;
		sf::Vector2f b2 = b1 + vec_from_len_angle(line_length / 2, line_angle + M_PI_4);

		sf::Vector2f c1 = a2;
		sf::Vector2f c2 = b1 + vec_from_len_angle(line_length / 2, line_angle - M_PI_4);

		pretransform_lines.push_back(a1);
		pretransform_lines.push_back(a2);
		//recurse(a1, a2, limit - 1);
		recurse(b1, b2, limit - 1);
		recurse(c1, c2, limit - 1);
	}
}

void YTree::generate(int recursions)
{
	pretransform_lines.clear();
	recurse(sf::Vector2f(), sf::Vector2f(0, -initial_height), recursions);
	lines.resize(pretransform_lines.size()); // resize array length, not scaling
	transfromLine();
}

const sf::VertexArray* YTree::getLine() const {
	return &lines;
}

void YTree::transfromLine() {
	for (size_t i = 0; i < pretransform_lines.size(); i++) {
		lines[i] = pretransform_lines[i];
		lines[i].position.x *= scale;
		lines[i].position.y *= scale;
		lines[i].position += origin;
	}
}

YTree::YTree(float initial_height, sf::RenderWindow* window, float scale, sf::Vector2f origin):
	initial_height(initial_height), window(window), scale(scale), origin(origin) {}

void YTree::setInitialHeight(float initial_height)
{
	this->initial_height = initial_height;
}

void YTree::setScale(float new_scale, sf::Vector2f scale_point)
{
	scaleMult(new_scale / scale, scale_point);
}

float YTree::getScale() const
{
	return scale;
}

void YTree::scaleMult(float scale_multi, sf::Vector2f scale_point)
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

void YTree::setOrigin(sf::Vector2f new_origin)
{
	origin = new_origin;
	transfromLine();
}

void YTree::translate(sf::Vector2f translation)
{
	origin += translation;
	transfromLine();
}

sf::Vector2f YTree::getOrigin() const
{
	return origin;
}
