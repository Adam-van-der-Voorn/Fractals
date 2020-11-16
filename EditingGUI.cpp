#include "EditingGUI.h"
#include "Editing.h"
#include "EditingState.h"
#include "EditableLine.h"
#include "EditableLineNode.h"
#include "debug_printing.h"
#include "StateMachine.h"
#include "SelLineWidget.h"
#include "LineFractal.h"
#include "WidgetHoriStack.h"
#include "AbsLine.h"
#include "vecutil.h"
#include "pi.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <memory>

EditingGUI::EditingGUI(EditingState* state, Editing* data)
	: state(state), editing(data)
{
	baseLine[0].position.x = editing->getBaseLine()->getNodeA()->getX();
	baseLine[0].position.y = editing->getBaseLine()->getNodeA()->getY();
	baseLine[0].color = sf::Color::Color(80, 80, 80);

	baseLine[1].position.x = editing->getBaseLine()->getNodeB()->getX();
	baseLine[1].position.y = editing->getBaseLine()->getNodeB()->getY();
	baseLine[1].color = sf::Color::Color(80, 80, 80);

	updateFractal();

	tGui = std::make_shared<tgui::Gui>(*state->getRenderWindow());
	setupTGUI(state->getRenderWindow()->getSize().x, state->getRenderWindow()->getSize().y);
	editing->addObserver(this);
}

void EditingGUI::onNotify(int event_num)
{
	auto event = static_cast<Editing::Event>(event_num);
	switch (event)
	{
	case Editing::LINES_CHANGED:
		updateLines();
		updateNodes();
		break;
	case Editing::SELECTION_CHANGED:
		updateNodes();
		{
			std::vector<tgui::Widget::Ptr> widgets;
			for (int selected_node_id : editing->getSelectedNodes()) {
				auto widget = std::make_shared<SelLineWidget>(editing, selected_node_id, editing->general_element_width, 50);
				widgets.push_back(widget->getPanel());
			}
			node_selections->swapStack(widgets);
		}
		break;
	case Editing::FRACTAL_CHANGED:
		updateFractal();
		break;
	case Editing::MOUSE_MOVED:
		updateNodes();
	case Editing::HOVERED_NODE_CHANGED:
		updateNodes();
	default:
		break;
	}
}

void EditingGUI::updateNodes()
{
	assert(editing->getNodes().size() % 2 == 0 && "uneven amount of nodes");
	nodes.resize(editing->getNodes().size());
	int i = 0;
	for (const auto& node_pair : editing->getNodes()) {
		nodes[i].setRadius(EditableLineNode::NODE_RADIUS);
		nodes[i].setFillColor(sf::Color::Transparent);
		nodes[i].setOutlineThickness(1.0f);
		nodes[i].setOrigin(sf::Vector2f(EditableLineNode::NODE_RADIUS, EditableLineNode::NODE_RADIUS));

		nodes[i].setPosition(sf::Vector2f(node_pair.second->getX(), node_pair.second->getY()));

		if (editing->getHoveredNode() == node_pair.first) { // node is selected and hovered
			nodes[i].setOutlineColor(sf::Color::Green);
		}
		else if (editing->getSelectedNodes().count(node_pair.first)) { // node is selected
			nodes[i].setOutlineColor(sf::Color::Red);
		}
		else if (node_pair.second->pointIntersection(editing->getMousePosInFrame().x, editing->getMousePosInFrame().y)) {
			nodes[i].setOutlineColor(sf::Color::White);
		}
		else {
			nodes[i].setOutlineColor(sf::Color::Transparent);
		}
		i++;
	}
}

void EditingGUI::updateLines()
{
	assert(editing->getNodes().size() % 2 == 0 && "uneven amount of nodes");
	int old_vertcount = nodeLines.getVertexCount();
	int new_vertcount = editing->getLines().size() * 6; // 6 bc two verts each on three lines making up an arrow.
	nodeLines.resize(new_vertcount);
	if (new_vertcount > old_vertcount) {
		for (int i = old_vertcount; i < new_vertcount; i++) {
			nodeLines[i].color = LINE_COL;
		}
	}
	int i = 0;
	for (const auto& line_pair : editing->getLines()) {
		double node_a_x = line_pair.second->getNodeA()->getX();
		double node_a_y = line_pair.second->getNodeA()->getY();
		double node_b_x = line_pair.second->getNodeB()->getX();
		double node_b_y = line_pair.second->getNodeB()->getY();

		// centerline
		nodeLines[i].position.x = node_a_x;
		nodeLines[i].position.y = node_a_y;
		i++;
		nodeLines[i].position.x = node_b_x;
		nodeLines[i].position.y = node_b_y;
		i++;

		// arrowheads
		constexpr double arrowhead_len = EditableLineNode::NODE_RADIUS;
		const double arrow_dir = lineAngle({ node_a_x, node_a_y, node_b_x, node_b_y });
		nodeLines[i].position.x = node_b_x;
		nodeLines[i].position.y = node_b_y;
		i++;
		nodeLines[i].position.x = node_b_x + lendirX(arrowhead_len, arrow_dir + m_pi4*3);
		nodeLines[i].position.y = node_b_y + lendirY(arrowhead_len, arrow_dir + m_pi4*3);
		i++;
		nodeLines[i].position.x = node_b_x;
		nodeLines[i].position.y = node_b_y;
		i++;
		nodeLines[i].position.x = node_b_x + lendirX(arrowhead_len, arrow_dir - m_pi4*3);
		nodeLines[i].position.y = node_b_y + lendirY(arrowhead_len, arrow_dir - m_pi4*3);
		i++;
	}
}

void EditingGUI::updateFractal() {
	int j = 0;
	std::vector<AbsLine> fractal_lines = editing->getFractal()->getLines();
	fractal.resize(fractal_lines.size() * 2);
	for (size_t i = 0; i < fractal.getVertexCount(); i += 2) {
		fractal[i] = sf::Vertex(sf::Vector2f(fractal_lines[j].x1, fractal_lines[j].y1));
		fractal[i + 1] = sf::Vertex(sf::Vector2f(fractal_lines[j].x2, fractal_lines[j].y2));
		j++;
	}
}

void EditingGUI::realignTGUI(int window_width, int window_height)
{
	right_panel->setSize(editing->right_panel_width, window_height);
	right_panel->setPosition(window_width - editing->right_panel_width, 0);
	display_button->setPosition(editing->general_padding, window_height - editing->general_padding - display_button->getSize().y);
	node_selections->getPanel()->setSize(editing->general_element_width, display_button->getPosition().y - (2 * editing->general_padding) - (measurement_fields[2]->getPosition().y + measurement_fields[2]->getSize().y));
}

void EditingGUI::setupTGUI(int window_width, int window_height)
{
	// right panel
	right_panel = tgui::Panel::create();
	tGui->add(right_panel);
	right_panel->setInheritedOpacity(0.8);

	// display button
	display_button = tgui::Button::create();
	right_panel->add(display_button);
	display_button->setSize(editing->general_element_width, 50);
	display_button->onClick(&StateMachine::changeState, state->getStateMachine(), "viewing");
	display_button->setText("Display fractal");

	// recursions field
	recursions_field = tgui::Panel::create();
	right_panel->add(recursions_field);
	recursions_field->setSize(editing->general_element_width, 20);
	recursions_field->setPosition(editing->general_padding, editing->general_padding);

	// recursions label
	recursions_label = tgui::Label::create();
	recursions_field->add(recursions_label);
	recursions_label->setSize({ "100%", "100%" });
	recursions_label->setPosition(0, 3); // slight adjustment so it lines up with input text
	recursions_label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	recursions_label->setText("Number of recursions");

	// recursions input
	recursions_input = tgui::EditBox::create();
	recursions_field->add(recursions_input);
	recursions_input->setSize({ 33, "100%" });
	recursions_input->setPosition({ recursions_field->getSize().x - recursions_input->getSize().x, 0 });
	recursions_input->setText(std::to_string(editing->getNumRecursions()));
	recursions_input->onReturnKeyPress(&EditingGUI::changeRecursionsField, this);

	// line actions field
	line_actions_field = tgui::Panel::create();
	right_panel->add(line_actions_field);
	line_actions_field->setSize(editing->general_element_width, 40);
	line_actions_field->setPosition({ editing->general_padding, tgui::bindBottom(recursions_field) + editing->general_padding });

	// add line button
	add_line_button = tgui::Button::create();
	line_actions_field->add(add_line_button);
	add_line_button->setSize((line_actions_field->getSize().x - editing->general_padding) / 2, "100%");
	add_line_button->setPosition(0, 0);
	add_line_button->setText("add line");
	add_line_button->onClick(&Editing::addLine, editing);

	// remove line button
	remove_line_button = tgui::Button::create();
	line_actions_field->add(remove_line_button);
	remove_line_button->setSize((editing->general_element_width - editing->general_padding) / 2, "100%");
	remove_line_button->setPosition({ tgui::bindRight(add_line_button) + editing->general_padding, 0 });
	remove_line_button->setText("remove line");

	// measurements block
	std::string label_names[] = { "Length", "Angle", "Position" };
	static const int no_measurement_fields = sizeof(measurement_inputs) / sizeof(tgui::EditBox::Ptr);
	for (int i = 0; i < no_measurement_fields; i++) {
		// measurement fields
		measurement_fields[i] = tgui::Panel::create();
		right_panel->add(measurement_fields[i]);
		measurement_fields[i]->setSize(editing->general_element_width, 20);

		// measurement labels
		measurement_labels[i] = tgui::Label::create();
		measurement_fields[i]->add(measurement_labels[i]);
		measurement_labels[i]->setText(label_names[i]);
		measurement_labels[i]->setSize("100%", "100%");
		measurement_labels[i]->setPosition(0, 3);

		// measurement inputs
		measurement_inputs[i] = tgui::EditBox::create();
		measurement_fields[i]->add(measurement_inputs[i]);
		measurement_inputs[i]->setSize(120, "100%");
		measurement_inputs[i]->setPosition(editing->general_element_width - measurement_inputs[i]->getSize().x, 0);
	}
	measurement_fields[0]->setPosition(editing->general_padding, tgui::bindBottom(line_actions_field) + editing->general_padding);
	measurement_fields[1]->setPosition(editing->general_padding, tgui::bindBottom(measurement_fields[0]) + editing->general_padding);
	measurement_fields[2]->setPosition(editing->general_padding, tgui::bindBottom(measurement_fields[1]) + editing->general_padding);


	// node selections
	node_selections = std::make_shared<WidgetHoriStack>();
	node_selections->setMargins(0, editing->general_padding);
	right_panel->add(node_selections->getPanel());

	node_selections->getPanel()->setPosition({ editing->general_padding, tgui::bindBottom(measurement_fields[2]) + editing->general_padding });

	realignTGUI(window_width, window_height);
}

void EditingGUI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(nodeLines);
	for (auto& node : nodes) {
		target.draw(node);
	}
	target.draw(baseLine);
	target.draw(fractal);
	tGui->draw();
}

void EditingGUI::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::Resized) {
		realignTGUI(event.size.width, event.size.height);
	}
	tGui->handleEvent(event);
}

void EditingGUI::changeRecursionsField() {
	tgui::String new_val = recursions_input->getText();
	if (new_val.length() == 0) {
		recursions_input->setText(std::to_string(editing->getNumRecursions()));
	}
	else {
		int new_num = new_val.toInt();
		if (new_num != editing->getNumRecursions()) {
			const int max_val = 20;
			if (new_num > max_val) {
				new_num = max_val;
				recursions_input->setText(std::to_string(new_num));
			}
			editing->setNumRecursions(new_num);
			editing->fractalChanged();
		}
	}
}

