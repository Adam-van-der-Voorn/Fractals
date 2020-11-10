#pragma once
#include "State.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Editing;
class EditingGUI;
class LineFractal;
class StateMachine;

class EditingState : public State
{
public:
	EditingState(StateMachine* state_machine, LineFractal* fractal, sf::RenderWindow* window);
	StateMachine* getStateMachine() const;
	sf::RenderWindow* getRenderWindow() const;
	void enter();
	void run();
	void exit();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void handleEvent(sf::Event& event);

private:
	StateMachine* state_machine;
	sf::RenderWindow* window;
	std::shared_ptr<Editing> data;
	std::shared_ptr<EditingGUI> view;
};

