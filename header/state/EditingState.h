#pragma once
#include "State.h"
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

class Editing;
class EditingGUI;
class StateMachine;

class EditingState : public State
{
public:
	EditingState(StateMachine* state_machine, sf::RenderWindow* window);

	/**
	\return a pointer to the state machine associated with this state
	**/
	StateMachine* getStateMachine() const;

	/**
	\return a pointer to the render window associated with this state
	**/
	sf::RenderWindow* getRenderWindow() const;

	/**
	\return a refrence to the data in this state
	**/
	const Editing* getData() const;

	// inherited via state
	void enter() override;
	void run() override;
	void exit() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(sf::Event& event) override;

private:
	StateMachine* state_machine;
	sf::RenderWindow* window;
	std::shared_ptr<Editing> data;
	std::shared_ptr<EditingGUI> view;
};

