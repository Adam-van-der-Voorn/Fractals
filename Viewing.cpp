#include "Viewing.h"
#include <SFML/Graphics/VertexArray.hpp>
#include "ZoomBox.h"
#include "LineFractal.h"
#include <iostream>

void f(int i) {
	std::cout << "jkfds" << i << std::endl;
}

Viewing::Viewing(sf::RenderWindow* window) {
	zoom_box = new ZoomBox(window);
	gui = new tgui::Gui{ *window };

	auto button = tgui::Button::create();
	gui->add(button);
	button->setText("swap");
	button->setEnabled(true);
	button->setSize(80, 40);
	button->setPosition(50, 50);
	button->onClick(f, 8);

	auto text = tgui::EditBox::create();
	gui->add(text);

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

ZoomBox* Viewing::getZoomBox() const
{
	return zoom_box;
}

void Viewing::draw(sf::RenderWindow* window) const {
	window->clear();
	gui->draw();
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

Viewing::~Viewing()
{
	delete zoom_box;
	delete gui;
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
				fractal->zoom(multi, mouse_pos);
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
					fractal->zoom(zoom_box->getZoomMulti(), zoom_box->getZoomPoint());
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


