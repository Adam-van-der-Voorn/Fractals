#include "EditingGUI.h"
#include "Editing.h"
#include "EditableLine.h"
#include "EditableLineNode.h"
#include "debug_printing.h"
#include "StateMachine.h"
#include <unordered_set>
#include <unordered_map>
#include <memory>

EditingGUI::EditingGUI(Editing * editing, sf::RenderWindow& window) : editing(editing)
{
	baseLine[0].position.x = editing->getBaseLine()->getNodeA()->getX();
	baseLine[0].position.y = editing->getBaseLine()->getNodeA()->getY();
	baseLine[0].color = sf::Color::Color(80, 80, 80);

	baseLine[1].position.x = editing->getBaseLine()->getNodeB()->getX();
	baseLine[1].position.y = editing->getBaseLine()->getNodeB()->getY();
	baseLine[1].color = sf::Color::Color(80, 80, 80);
	tGui = std::make_shared<tgui::Gui>(window);
	setupTGUI(window.getSize().x, window.getSize().y);
	editing->addObserver(this);
}

void EditingGUI::onNotify(int event_num)
{
	auto event = static_cast<Editing::Event>(event_num);
	switch (event)
	{
	case Editing::LINES_CHANGED:
		// make data structs the correct size
		assert(editing->getNodes().size() % 2 == 0 && "uneven amount of nodes");
		nodeLines.resize(editing->getNodes().size()); // using num of nodes instead of num of lines because each line has two verts
		nodes.resize(editing->getNodes().size());
		updateLines();
		updateNodes();
		break;
	case Editing::SELECTION_CHANGED:
		assert(editing->getNodes().size() % 2 == 0 && "uneven amount of nodes");
		nodes.resize(editing->getNodes().size());
		updateNodes();
		break;
	default:
		assert(false && "bad enum somehow");
		break;
	}
}

void EditingGUI::updateNodes()
{
	int i = 0;
	for (const auto& node_pair : editing->getNodes()) {
		nodes[i].setPosition(sf::Vector2f(node_pair.second->getX(), node_pair.second->getY()));
		nodes[i].setRadius(EditableLineNode::NODE_RADIUS);
		nodes[i].setFillColor(sf::Color::Transparent);
		nodes[i].setOutlineColor(sf::Color::White);
		nodes[i].setOutlineThickness(1.0f);
		nodes[i].setOrigin(sf::Vector2f(EditableLineNode::NODE_RADIUS, EditableLineNode::NODE_RADIUS));
		if (editing->getSelectedNodes().count(node_pair.first)) { // node is selected
			nodes[i].setOutlineColor(sf::Color::Red);
		}
		else {
			nodes[i].setOutlineColor(sf::Color::White);
		}
		i++;
	}
}

void EditingGUI::updateLines()
{
	int i = 0;
	for (const auto& line_pair : editing->getLines()) {
		std::shared_ptr<EditableLineNode> node_a = line_pair.second->getNodeA();
		nodeLines[i].position.x = node_a->getX();
		nodeLines[i].position.y = node_a->getY();
		i++;
		std::shared_ptr<EditableLineNode> node_b = line_pair.second->getNodeB();
		nodeLines[i].position.x = node_b->getX();
		nodeLines[i].position.y = node_b->getY();
		i++;
	}
}

void EditingGUI::realignTGUI(int window_width, int window_height)
{
	right_panel->setSize(editing->right_panel_width, window_height);
	right_panel->setPosition(window_width - editing->right_panel_width, 0);
	display_button->setPosition(editing->general_padding, window_height - editing->general_padding - display_button->getSize().y);
	node_selections->setSize(editing->general_element_width, display_button->getPosition().y - (2 * editing->general_padding) - (measurement_fields[2]->getPosition().y + measurement_fields[2]->getSize().y));
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
	display_button->onClick(&StateMachine::changeState, editing->getStateMachine(), "viewing");
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
	node_selections = tgui::ScrollablePanel::create();
	right_panel->add(node_selections);
	node_selections->setPosition({ editing->general_padding, tgui::bindBottom(measurement_fields[2]) + editing->general_padding });

	realignTGUI(window_width, window_height);
}

void EditingGUI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(nodeLines);
	for (auto& node : nodes) {
		target.draw(node);
	}
	target.draw(baseLine);
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

