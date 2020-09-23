#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "LineFractal.h"
#include "ZoomBox.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#ifdef _DEBUG
#  include <iostream>
#  define PRINT(x) std::cout << x << std::endl;
#else
#  define PRINT(x) while(false){}
#endif

//TODO
// - add undo
// - fix floating point innaccuaracies when zooming in very large amounts

float scale_multi = 0.2;

sf::ContextSettings settings(0, 0, 8);
sf::RenderWindow window(sf::VideoMode(900, 600), "asd", sf::Style::Default, settings);
LineFractal fractal(0, 0, 0, -150);

ZoomBox zoom_box(&window);

void draw() {
	window.clear();
	window.draw(*fractal.getFractal());
	if (zoom_box.isActive()) {
		window.draw(*zoom_box.getRect());
		sf::VertexArray line1(sf::LineStrip, 2);
		sf::VertexArray line2(sf::LineStrip, 2);
		line1[0].position = sf::Vector2f(0, 0);
		line2[0].position = sf::Vector2f(window.getSize().x, 0);
		line1[1].position = zoom_box.getZoomPoint();
		line2[1].position = zoom_box.getZoomPoint();
		window.draw(line1);
		window.draw(line2);
	}
	window.display();
}

int main()
{
	fractal.setOrigin(sf::Vector2f(450, 598));
	fractal.generate(5);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					zoom_box.setStartPoint(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
				}
				if (event.mouseButton.button == sf::Mouse::Right) {
					float multi = 2;
					sf::Vector2f mouse_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
					fractal.zoom(multi, mouse_pos);
				}
			}
			else if (event.type == sf::Event::MouseMoved) {
				if (zoom_box.isActive()) {
					zoom_box.setEndPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					PRINT("rect pos: " << zoom_box.getRect()->getPosition().x << ", " << zoom_box.getRect()->getPosition().x << ")");
					PRINT("rect size: " << zoom_box.getRect()->getSize().x << ", " << zoom_box.getRect()->getSize().x << ")");
					if (zoom_box.getRect()->getSize().x != 0 && zoom_box.getRect()->getSize().y != 0) {
						fractal.zoom(zoom_box.getZoomMulti(), zoom_box.getZoomPoint());
					}	
					zoom_box.setUnactive();
				}
			}
		}
		draw();
	}
	return 0;
}
