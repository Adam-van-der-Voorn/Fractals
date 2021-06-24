#include "EditingState.h"
#include "Editing.h"
#include "EditingGUI.h"

EditingState::EditingState(StateMachine * state_machine, sf::RenderWindow * window)
	: state_machine(state_machine), window(window)
{
	data = std::make_shared<Editing>(this);
	view = std::make_shared<EditingGUI>(this, data.get());
	data->addObserver(view.get());
}

StateMachine* EditingState::getStateMachine() const
{
	return state_machine;
}

sf::RenderWindow* EditingState::getRenderWindow() const
{
	return window;
}

const Editing* EditingState::getData() const
{
	return data.get();
}

void EditingState::enter()
{
}

void EditingState::run()
{
}

void EditingState::exit()
{
}

void EditingState::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*view, states);
}

void EditingState::handleEvent(sf::Event & event)
{
	data->handleEvent(event);
	view->handleEvent(event);
}
