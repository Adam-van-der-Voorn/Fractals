#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "LineFractal.h"
#include "ZoomBox.h"
#include "Viewer.h"
#include <SFML/Graphics.hpp>
#include <vector>

#ifdef _DEBUG
#  include <iostream>
#  define PRINT(x) std::cout << x << std::endl;
#else
#  define PRINT(x) while(false){}
#endif

//TODO
// - add undo
// - fix floating point innaccuaracies when zooming in very large amounts

enum State {
	VIEW, EDIT
};

int main()
{
	State state = VIEW;
	sf::ContextSettings settings(0, 0, 8);
	sf::RenderWindow window(sf::VideoMode(900, 600), "asd", sf::Style::Default, settings);
	Viewer viewer(&window);
	LineFractal fractal(0, 0, 0, -300, 1, sf::Vector2f(450, 598));
	viewer.addFractal(&fractal);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (state == VIEW) {
				if (event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						viewer.getZoomBox()->setStartPoint(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
					}
					if (event.mouseButton.button == sf::Mouse::Right) {
						float multi = 2;
						sf::Vector2f mouse_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
						fractal.zoom(multi, mouse_pos);
					}
				}
				else if (event.type == sf::Event::MouseMoved) {
					if (viewer.getZoomBox()->isActive()) {
						viewer.getZoomBox()->setEndPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
					}
				}
				else if (event.type == sf::Event::MouseButtonReleased) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						if (viewer.getZoomBox()->getRect()->getSize().x != 0 && viewer.getZoomBox()->getRect()->getSize().y != 0) {
							fractal.zoom(viewer.getZoomBox()->getZoomMulti(), viewer.getZoomBox()->getZoomPoint());
						}
						viewer.getZoomBox()->setUnactive();
					}
				}
			}
		}
		viewer.draw(&window);
	}
	return 0;
}
