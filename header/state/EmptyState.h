#pragma once
#include "State.h"

/**
implementation of state where every function does nothing.
Useful as a base state that the program starts in.
**/
class EmptyState : public State
{
public:
	void enter();
	void run();
	void exit();
	void handleEvent(sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
};

