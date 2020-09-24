#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>

class LineFractal
{
public:
	LineFractal(double x1, double y1, double x2, double y2, float scale = 1, sf::Vector2f origin = sf::Vector2f(0, 0));

	// getters & setters
	void setScale(float new_scale, sf::Vector2f scale_point);
	float getScale() const;
	void setOrigin(sf::Vector2f origin);
	sf::Vector2f getOrigin() const;

	void zoom(float zoom_multi, sf::Vector2f zoom_point);
	void translate(sf::Vector2f translation);

	void generate(int recursions);

	const sf::VertexArray* getFractal() const;
private:

	sf::Vector2f origin, pa, pb;
	float scale;

	std::vector<sf::Vertex> base_lines;
	sf::VertexArray final_lines = sf::VertexArray(sf::Lines);

	void recurse(sf::Vector2f start_point, sf::Vector2f end_point, int limit);
	void transfromLine();

	struct Line {
		double distance1;
		double angle1;
		double distance2;
		double angle2;
		bool recursing;
	};

	std::vector<Line> derived_lines;
};


