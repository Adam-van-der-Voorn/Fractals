#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

class LineFractal;
class ZoomBox;

class Viewer
{
public:
	Viewer(const sf::RenderWindow* window);
	~Viewer();

	void draw(sf::RenderWindow* window) const;
	void addFractal(LineFractal* fractal);

	//getters & setters
	ZoomBox* getZoomBox() const;

private:
	std::vector<LineFractal*> fractals;
	ZoomBox* zoom_box;
};

