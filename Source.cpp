#pragma once
#include "LineFractal.h"
#include "Viewing.h"
#include "EditingState.h"
#include "StateMachine.h"
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include "debug_printing.h"

#include "vecutil.h"

//TODO
// - add undo
// - fix floating point innaccuaracies when zooming in very large amounts
// - fix bug where new widgets in widget stack dont go to the bottom
// - make the gui not an event handler
// - stop the fields letting the lines do illegal moves
//      - some kind of message popup system to inform the user its illegal would b good.
//
sf::ContextSettings settings(0, 0, 8);
sf::RenderWindow window(sf::VideoMode(900, 600), "asd", sf::Style::Default, settings);

int main()
{
	LineFractal fractal({0, 0, 0, 0});
	StateMachine state_machine;
	std::shared_ptr<State> viewing_state = std::make_shared<Viewing>(window);
	std::shared_ptr<State> editing_state = std::make_shared<EditingState>(&state_machine, &fractal, &window);
	state_machine.addState("viewing", viewing_state.get());
	state_machine.addState("editing", editing_state.get());
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
		state_machine.getState()->draw(window);
		window.display();
	}
	return 0;
}
