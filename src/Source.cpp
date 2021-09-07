#include "LineFractal.h"
#include "ViewingState.h"
#include "EditingState.h"
#include "StateMachine.h"
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include "debug_printing.h"

#include "vecutil.h"

#include "Vec2.h"
#include "degrad.h"

sf::ContextSettings settings(0, 0, 8);
sf::RenderWindow window(sf::VideoMode(900, 600), "asd", sf::Style::Default, settings);

int main()
{	
	PRINT("Entered");
	StateMachine state_machine;
	EditingState editing_state(&state_machine, &window);
	ViewingState viewing_state(&state_machine, editing_state.getData(), &window);
	state_machine.addState("viewing", &viewing_state);
	state_machine.addState("editing", &editing_state);
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
