#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "LineFractal.h"
#include "ZoomBox.h"
#include "Viewing.h"
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
State* state;

int main()
{
	sf::ContextSettings settings(0, 0, 8);
	sf::RenderWindow window(sf::VideoMode(900, 600), "asd", sf::Style::Default, settings);
	
	Viewing viewing(&window);
	LineFractal fractal(0, 0, 0, -300, 1, sf::Vector2f(450, 598));
	viewing.addFractal(&fractal);

	state = &viewing;
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			state->handleEvent(event);
		}
		state->draw(&window);
	}
	return 0;
}
