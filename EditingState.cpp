#include "EditingState.h"
#include "Editing.h"
#include "EditingGUI.h"

EditingState::EditingState(StateMachine * state_machine, LineFractal * fractal, sf::RenderWindow * window)
	: state_machine(state_machine), window(window)
{
	data = std::make_shared<Editing>(this, fractal);
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
	target.draw(*view);
}

void EditingState::handleEvent(sf::Event & event)
{
	data->handleEvent(event);
	view->handleEvent(event);
}
