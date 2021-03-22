#include "ViewingGUI.h"
#include "ViewingState.h"
#include "Viewing.h"
#include "AbsLine.h"
#include "LineFractal.h"
#include "StateMachine.h"
#include "ZoomBox.h"
#include "RightAngleRect.h"
#include "sfml_conversions.h"
#include "debug_printing.h"

ViewingGUI::ViewingGUI(ViewingState* state, Viewing* data)
	: data(data)
{
	gui = std::make_unique<tgui::Gui>(*state->getRenderWindow());
	test = tgui::Button::create();
	gui->add(test);
	test->setSize(40, 20);
	test->setText("back");
	test->onClick(&StateMachine::changeState, state->getStateMachine(), "editing");
	zoom_box_shape.setFillColor(sf::Color::Transparent);
	zoom_box_shape.setOutlineThickness(1.0f);
	zoom_box_shape.setOutlineColor(sf::Color::Red);
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
		fractal_lines[i] = sf::Vertex(sf::Vector2f(raw_lines[j].back.x, raw_lines[j].back.y));
		fractal_lines[i + 1] = sf::Vertex(sf::Vector2f(raw_lines[j].head.x, raw_lines[j].head.y));
		j++;
	}
}

void ViewingGUI::onNotify(Viewing* subject, int event_num)
{
	auto event = static_cast<Viewing::Event>(event_num);
	if (event == Viewing::ZOOM_BOX_CHANGE) {
		RightAngleRect box = data->getZoomBox()->getBox();
		zoom_box_shape.setPosition(sf::Vector2f(box.tL().x, box.tL().y));
		zoom_box_shape.setSize(sf::Vector2f(box.size().x, box.size().y));

	}
	else if (event == Viewing::VIEW_CHANGE) {
		updateFractal();
	}
}

#include "Circle.h" //tmp
void ViewingGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (DEBUG) {
		static sf::VertexArray frac_shadow(sf::Lines);
		static RightAngleRect bounds({ 0,0 }, { 0,0 });
		static std::vector<sf::RectangleShape> bounds_shape_list;
		if (bounds != data->getFractal()->getBoundsInstance(data->getFractal()->getBaseLine())) {
			// shadow
			int j = 0;
			std::vector<AbsLine> raw_lines;
			data->getFractal()->generateIter(9999, raw_lines);
			frac_shadow.resize(raw_lines.size() * 2);
			for (size_t i = 0; i < frac_shadow.getVertexCount(); i += 2) {
				frac_shadow[i] = sf::Vertex(sf::Vector2f(raw_lines[j].back.x, raw_lines[j].back.y), sf::Color(255, 255, 255, 80));
				frac_shadow[i + 1] = sf::Vertex(sf::Vector2f(raw_lines[j].head.x, raw_lines[j].head.y), sf::Color(255, 255, 255, 80));
				j++;
			}

			// bounds
			bounds_shape_list.clear();
			for (const AbsLine& line : data->getFractal()->getLines()) {
				RightAngleRect r = data->getFractal()->getBoundsInstance(line);
				sf::RectangleShape bounds_shape;
				bounds_shape.setFillColor(sf::Color(255, 0, 255, 30));
				bounds_shape.setSize(sfVecFromVec2(r.size()));
				bounds_shape.setPosition(sfVecFromVec2(r.tL()));
				bounds_shape_list.push_back(bounds_shape);
			}
		}
		for (const sf::RectangleShape& shape : bounds_shape_list) {
			target.draw(shape);
		}
		target.draw(frac_shadow);
	}

	auto fractal_transform = sf::Transform()
		.scale(
			data->getFractalZoom(),
			data->getFractalZoom()
		)
		.translate(sfVecFromVec2(data->getFractalOffset()))
		.translate(450, 300);
	target.draw(fractal_lines, fractal_transform);

	if (data->getZoomBox()->isActive() && data->getZoomBox()->isRect()) {
		target.draw(zoom_box_shape);
	}
	gui->draw();
}
