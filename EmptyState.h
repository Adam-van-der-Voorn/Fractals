#pragma once
#include "State.h"

class EmptyState : public State
{
public:
	void enter();
	void run();
	void exit();
	void handleEvent(sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
};

