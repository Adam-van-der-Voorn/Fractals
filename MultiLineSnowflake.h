#pragma once
#include <SFML/Graphics.hpp>

class MultiLineSnowflake
{
public:
	MultiLineSnowflake(sf::Vector2f pa, sf::Vector2f pb, sf::RenderWindow* window, float scale = 1, sf::Vector2f origin = sf::Vector2f(0, 0)) :
		pa(pa), pb(pb), window(window), scale(scale), origin(origin) {}

	// getters & setters //
	void setPointA(sf::Vector2f pa);
	sf::Vector2f getPointA() const;
	void setPointB(sf::Vector2f pb);
	sf::Vector2f getPointB() const;
	void setScale(float new_scale, sf::Vector2f scale_point);
	float getScale() const;
	void scaleMult(float scale_multi, sf::Vector2f scale_point);
	void setOrigin(sf::Vector2f origin);
	void translate(sf::Vector2f translation);
	sf::Vector2f getOrigin() const;

	void generate(int recursions);
	const sf::VertexArray* getLine() const;
private:
	void generate();
	sf::Vector2f origin, pa, pb;
	float scale;
	sf::RenderWindow* window;
	std::vector<sf::Vertex> relative_line;
	sf::VertexArray line = sf::VertexArray(sf::Lines);
	void recurse(sf::Vector2f start_point, sf::Vector2f end_point, int limit);
	void transfromLine();
};


