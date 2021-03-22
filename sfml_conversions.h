#pragma once
#include "Vec2.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

inline Vec2 vec2FromSF(const sf::Vector2f& v)
{
	return { static_cast<double>(v.x), static_cast<double>(v.y) };
}

inline Vec2 vec2FromSF(const sf::Vector2i& v)
{
	return { static_cast<double>(v.x), static_cast<double>(v.y) };
}

inline Vec2 vec2FromSF(const sf::Vector2u& v)
{
	return { static_cast<double>(v.x), static_cast<double>(v.y) };
}

inline sf::Transform sfTransformFromVec2(const Vec2& v) {
	sf::Transform tmp;
	tmp.translate(v.x, v.y);
	return tmp;
}

inline sf::Vector2f sfVecFromVec2(const Vec2& v) {
	return sf::Vector2f( v.x, v.y );
}