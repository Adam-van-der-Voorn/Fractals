#pragma once
#include <SFML/Graphics.hpp>

class YTree
{
public:
	YTree(float initial_height, sf::RenderWindow* window, float scale = 1, sf::Vector2f origin = sf::Vector2f(0, 0));

	// getters & setters //
	void setInitialHeight(float height);
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
	sf::Vector2f origin;
	float initial_height, scale;
	sf::RenderWindow* window;
	std::vector<sf::Vertex> pretransform_lines;
	sf::VertexArray lines = sf::VertexArray(sf::Lines);
	void recurse(sf::Vector2f start_point, sf::Vector2f end_point, int limit);
	void transfromLine();
};

