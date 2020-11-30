#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "EventHandler.h"
/**

**/
class State : public sf::Drawable, public EventHandler {
public:
	/**
	called by the StateMachine class when the state is entered
	**/
	virtual void enter() = 0;

	/**
	function meant to be called every gameloop when the state is active
	**/
	virtual void run() = 0;

	/**
	called by the StateMachine class when the state is exited
	**/
	virtual void exit() = 0;

	// Inherited via EventHandler
	virtual void handleEvent(sf::Event& event) override = 0;

	// Inherited via sf::Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;

};