#include "Viewing.h"
#include "ViewingState.h"
#include "ZoomBox.h"
#include "LineFractal.h"
#include "StateMachine.h"
#include "RightAngleRect.h"
#include "sfml_conversions.h"
#include "debug_printing.h"
#include <cassert>
#include <iostream>

Viewing::Viewing(ViewingState* state, LineFractal* fractal) :
	fractal(fractal), state(state)
{
	zoom_box = new ZoomBox(state->getRenderWindow());
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

void Viewing::changeViewWindow(const RightAngleRect& view_multi)
{
	double scale_multi = view_multi.size().x / state->getRenderWindow()->getSize().x;
	Vec2 shift_multi = {
		view_multi.left() / state->getRenderWindow()->getSize().x,
		view_multi.top() / state->getRenderWindow()->getSize().y
	};
	current_view = RightAngleRect::fromSize(
		/* position */ current_view.tL() + Vec2(shift_multi.x * current_view.size().x, shift_multi.y * current_view.size().y),
		/* size     */ current_view.size() * scale_multi
	);

	fractal_zoom = state->getRenderWindow()->getSize().x / current_view.size().x;
	fractal->setDefinition(current_view.size().x / state->getRenderWindow()->getSize().x);
	fractal_offset = ((current_view.tL()) * -1);  // go to top left

	fractal->setView(current_view.translate({ -450, -300 }));
}

double Viewing::getFractalZoom() const
{
	return fractal_zoom;
}

Vec2 Viewing::getFractalOffset() const
{
	return fractal_offset;
}

void Viewing::resetFractalTransform()
{
	fractal_zoom = 1;
	fractal_offset = Vec2(0, 0);
	fractal->setDefinition(1);
	current_view = RightAngleRect::fromSize({0, 0}, vec2FromSF(state->getRenderWindow()->getSize()));
	fractal->setView(current_view.translate(Vec2(450, 300)));
}

void Viewing::centerFractal()
{
	RightAngleRect to_center = fractal->getBoundsInstance(fractal->getBaseLine());
	Vec2 window_size = vec2FromSF(state->getRenderWindow()->getSize());
	Vec2 position = (window_size - to_center.size()) / 2;
	Vec2 translation = position - to_center.tL();
	fractal->translate(translation);
}

void Viewing::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			zoom_box->setStartPoint({ event.mouseButton.x, event.mouseButton.y });
			notifyAll(ZOOM_BOX_CHANGE);
		}
		if (event.mouseButton.button == sf::Mouse::Right) {
			resetFractalTransform();
		}
	}
	else if (event.type == sf::Event::MouseMoved) {

		if (zoom_box->isActive()) {
			zoom_box->setEndPoint({ event.mouseMove.x, event.mouseMove.y });
			notifyAll(ZOOM_BOX_CHANGE);
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (zoom_box->isRect()) {
				auto view_multi = zoom_box->getBox();
				changeViewWindow(view_multi);
				fractal->generate();
				notifyAll(VIEW_CHANGE);
			}
			zoom_box->setUnactive();
		}
	}
}

