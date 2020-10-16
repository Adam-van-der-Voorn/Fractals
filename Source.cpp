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

#include "vecutil.h"

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
	PRINT("e  " << angleBetweenAB(-1, 0, 1, 0) << " 0");
	PRINT("se " << angleBetweenAB(-1, -1, 1, 1) << " pi/4 : " << m_pi4);
	PRINT("s  " << angleBetweenAB(0, -1, 0, 1) << " pi/2 : " << m_pi2);
	PRINT("sw " << angleBetweenAB(1, -1, -1, 1) << " 3pi/4 : " << 3 * m_pi4);
	PRINT("w  " << angleBetweenAB(1, 0, -1, 0) << " pi : " << m_pi);
	PRINT("nw " << angleBetweenAB(1, 1, -1, -1) << " 5pi/4 : " << 5 * m_pi4);
	PRINT("n  " << angleBetweenAB(0, 1, 0, -1) << " 3pi/2 : " << 3 * m_pi2);
	PRINT("ne " << angleBetweenAB(-1, 1, 1, -1) << " 7pi/4 : " << 7 * m_pi4);
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
