#include "Viewer.h"
#include <SFML/Graphics/VertexArray.hpp>
#include "ZoomBox.h"
#include "LineFractal.h"
Viewer::Viewer(const sf::RenderWindow* window) {
	zoom_box = new ZoomBox(window);
}

void Viewer::addFractal(LineFractal* fractal) {
	fractal->generate(5);
	fractals.push_back(fractal);
}

ZoomBox* Viewer::getZoomBox() const
{
	return zoom_box;
}

void Viewer::draw(sf::RenderWindow* window) const {
	window->clear();
	for (LineFractal* f : fractals) {
		const sf::VertexArray* frac_lines = f->getFractal();
		window->draw(*frac_lines);
	}
	
	if (zoom_box->isActive()) {
		const sf::RectangleShape* rect = zoom_box->getRect();
		window->draw(*rect);
	}
	window->display();
}

Viewer::~Viewer()
{
	delete zoom_box;
}