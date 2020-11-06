#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>

class State : sf::Drawable {
public:
	virtual void enter() = 0;
	virtual void run() = 0;
	virtual void exit() = 0;
	virtual void handleEvent(sf::Event& event) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;

};