#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

struct Vec2 {
	double x;
	double y;

	static Vec2 sf(const sf::Vector2f v);
	static Vec2 sf(const sf::Vector2i v);
	static Vec2 sf(const sf::Vector2u v);
	static Vec2 prim(unsigned int x, unsigned int y);
	static Vec2 prim(int x, int y);
	static Vec2 fromLenDir(double len, double dir);

	void operator += (const Vec2& other);
	void operator -= (const Vec2& other);
	Vec2 operator + (const Vec2& other) const;
	Vec2 operator - (const Vec2& other) const;
	Vec2 operator / (double other) const;
	Vec2 operator * (double other) const;
	bool operator == (const Vec2& other) const;

	sf::Transform toSFTransform() const;


};