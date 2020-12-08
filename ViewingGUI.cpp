#include "ViewingGUI.h"
#include "ViewingState.h"
#include "Viewing.h"
#include "AbsLine.h"
#include "LineFractal.h"
#include "StateMachine.h"

ViewingGUI::ViewingGUI(ViewingState* state, Viewing* data)
	: data(data)
{
	gui = std::make_unique<tgui::Gui>(*state->getRenderWindow());
	test = tgui::Button::create();
	gui->add(test);
	test->setSize(40, 20);
	test->setText("back");
	test->onClick(&StateMachine::changeState, state->getStateMachine(), "editing");
	updateFractal();
}

void ViewingGUI::handleEvent(sf::Event & event)
{
	gui->handleEvent(event);
}

void ViewingGUI::updateFractal() {
	int j = 0;
	std::vector<AbsLine> raw_lines = data->getFractal()->getLines();
	fractal_lines.resize(raw_lines.size() * 2);
	for (size_t i = 0; i < fractal_lines.getVertexCount(); i += 2) {
		fractal_lines[i] = sf::Vertex(sf::Vector2f(raw_lines[j].back_x, raw_lines[j].back_y));
		fractal_lines[i + 1] = sf::Vertex(sf::Vector2f(raw_lines[j].head_x, raw_lines[j].head_y));
		j++;
	}
}

void ViewingGUI::onNotify(Viewing* subject, int event_num)
{
	
}

void ViewingGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fractal_lines);
	gui->draw();
}
