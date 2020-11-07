#include "Viewing.h"
#include <SFML/Graphics/VertexArray.hpp>
#include "ZoomBox.h"
#include "LineFractal.h"
#include "StateMachine.h"
#include <iostream>

Viewing::Viewing(sf::RenderWindow& window)
{
	zoom_box = new ZoomBox(&window);
	gui = new tgui::Gui{ window };

	auto swap_button = tgui::Button::create();
	gui->add(swap_button);
	swap_button->setText("swap");
	swap_button->setSize(80, 40);
	swap_button->setPosition(50, 50);
	//swap_button->onClick(cs_editing);

	auto text = tgui::EditBox::create();
	gui->add(text);
}

Viewing::~Viewing()
{
	delete zoom_box;
	delete gui;
}

void Viewing::incFractal() {
	static int i = 0;
	fractals[0]->generate(i);
	i++;
}


void Viewing::addFractal(LineFractal* fractal) {
	fractal->generate(5);
	fractals.push_back(fractal);
}

void Viewing::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	gui->draw();
	for (auto f : fractals) {
		target.draw(f->getFractal());
	}

	if (zoom_box->isActive()) {
		const sf::RectangleShape* rect = zoom_box->getRect();
		target.draw(*rect);
	}
}

ZoomBox* Viewing::getZoomBox() const
{
	return zoom_box;
}

void Viewing::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			zoom_box->setStartPoint(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		}
		if (event.mouseButton.button == sf::Mouse::Right) {
			float multi = 2;
			sf::Vector2f mouse_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
			for (LineFractal* fractal : fractals) {
				fractal->zoom(multi, mouse_pos.x, mouse_pos.y);
			}
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		if (zoom_box->isActive()) {
			zoom_box->setEndPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (zoom_box->getRect()->getSize().x != 0 && zoom_box->getRect()->getSize().y != 0) {
				for (LineFractal* fractal : fractals) {
					fractal->zoom(zoom_box->getZoomMulti(), zoom_box->getZoomPoint().x, zoom_box->getZoomPoint().y);
				}
			}
			zoom_box->setUnactive();
		}
	}
	gui->handleEvent(event);
}

void Viewing::enter()
{
}

void Viewing::run()
{
}

void Viewing::exit()
{
}


