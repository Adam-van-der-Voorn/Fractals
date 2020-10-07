#pragma once
#include "LineFractal.h"
#include "Viewing.h"
#include "Editing.h"
#include "StateMachine.h"
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include "debug_printing.h"

//TODO
// - add undo
// - fix floating point innaccuaracies when zooming in very large amounts
sf::ContextSettings settings(0, 0, 8);
sf::RenderWindow window(sf::VideoMode(900, 600), "asd", sf::Style::Default, settings);

StateMachine state_machine;
State* viewing;
State* editing;
LineFractal fractal(0, 0, 0, -100);

int main()
{	
	editing = new Editing(window);
	viewing = new Viewing(window);
	state_machine.addState("viewing", viewing);
	state_machine.addState("editing", editing);
	state_machine.changeState("editing");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			state_machine.getState()->handleEvent(event);
		}
		state_machine.getState()->run();
		window.clear();
		state_machine.getState()->drawTo(window);
		window.display();
	}
	delete viewing;
	delete editing;
	return 0;
}
