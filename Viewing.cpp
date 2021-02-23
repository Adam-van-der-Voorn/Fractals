#include "Viewing.h"
#include "ViewingState.h"
#include <SFML/Graphics/VertexArray.hpp>
#include "ZoomBox.h"
#include "LineFractal.h"
#include "StateMachine.h"
#include <iostream>

Viewing::Viewing(ViewingState* state, LineFractal* fractal) :
	fractal(fractal), state(state)
{
	zoom_box = new ZoomBox(state->getRenderWindow());
	//global_offset = Vec2(state->getRenderWindow()->getSize()) / 2;
}

Viewing::~Viewing()
{
	delete zoom_box;
}

ZoomBox* Viewing::getZoomBox() const
{
	return zoom_box;
}

const LineFractal * Viewing::getFractal() const
{
	return fractal;
}

void Viewing::updateFractalBounds()
{
	fractal->updateBounds();
}

Vec2 Viewing::getGlobalOffset() const
{
	return global_offset;
}

void Viewing::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			zoom_box->setStartPoint(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		}
		/*if (event.mouseButton.button == sf::Mouse::Right) {
			float multi = 2;
			sf::Vector2f mouse_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
			fractal->zoom(multi, mouse_pos.x, mouse_pos.y);
		}*/
	}
	else if (event.type == sf::Event::MouseMoved) {
		if (zoom_box->isActive()) {
			zoom_box->setEndPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			auto rect_pos = zoom_box->getRect()->getPosition();
			auto rect_size = zoom_box->getRect()->getSize();
			if (zoom_box->getRect()->getSize().x != 0 && zoom_box->getRect()->getSize().y != 0) {
				fractal->setView(rect_pos.y, rect_pos.x, rect_pos.y + rect_size.y, rect_pos.x + rect_size.x);
				fractal->generate();
			}
			zoom_box->setUnactive();
		}
	}
}


