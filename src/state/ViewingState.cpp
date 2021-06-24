#include "ViewingState.h"
#include "Viewing.h"
#include "ViewingGUI.h"
#include "LineFractal.h"
#include "Editing.h"

ViewingState::ViewingState(StateMachine* state_machine, const Editing* editing, sf::RenderWindow* window)
	: state_machine(state_machine), editing(editing), window(window)
{
	data = std::make_shared<Viewing>(this);
	view = std::make_shared<ViewingGUI>(this, data.get());
	data->addObserver(view.get());
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
	const LineFractal& f = editing->getFractal();
	data->setFractal(f);
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
