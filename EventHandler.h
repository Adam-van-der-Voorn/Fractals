#pragma once
#include <SFML/Window/Event.hpp>
class EventHandler 
{
public:

	/**
	handles sfml events such as mouse clicks, window resizes etc.
	basically gives the extending class the abilty to deal with sfml events.
	\param event the event to handle
	**/
	virtual void handleEvent(sf::Event& event) = 0;
};