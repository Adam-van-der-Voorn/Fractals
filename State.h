#pragma once
#include <SFML/Window/Event.hpp>
#include "Drawable.h"

class State : public Drawable {
public:
	virtual void enter() = 0;
	virtual void run() = 0;
	virtual void exit() = 0;
	virtual void handleEvent(sf::Event& event) = 0;
};