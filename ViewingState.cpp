#include "ViewingState.h"
#include "Viewing.h"
#include "ViewingGUI.h"

ViewingState::ViewingState(StateMachine* state_machine, LineFractal* fractal, sf::RenderWindow* window)
	: state_machine(state_machine), window(window)
{
	data = std::make_shared<Viewing>(this, fractal);
	view = std::make_shared<ViewingGUI>(this, data.get());
}

StateMachine* ViewingState::getStateMachine() const
{
	return state_machine;
}

sf::RenderWindow* ViewingState::getRenderWindow() const
{
	return window;
}

void ViewingState::enter()
{
	view->updateFractal();
}

void ViewingState::run()
{
}

void ViewingState::exit()
{
}

void ViewingState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*view);
}

void ViewingState::handleEvent(sf::Event& event)
{
	data->handleEvent(event);
	view->handleEvent(event);
}
