#include "Editing.h"
#include "StateMachine.h"
#include "LineFractal.h"
#include "debug_printing.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "vertarrutil.h"
#include "AbsLine.h"
#include "EditableLineNode.h"
#include <cstdlib>
#include <string>

extern StateMachine state_machine;
extern LineFractal fractal;

void cs_viewing() {
	state_machine.changeState("viewing");
}

int getID() {
	static int i = 0;
	return i++;
}

void addLine(sf::Vector2i* position, std::unordered_map<int, std::shared_ptr<EditableLineNode>>* nodes, std::unordered_map<int, std::shared_ptr<EditableLine>>* lines) {
	float length = rand() % 50 + 50; // random length from 50 - 100
	float angle = ((float)(rand() % (3141 * 2))) / 100; // random angle in radians from 0 - ~2pi
	float x = lendirX(length, angle);
	float y = lendirY(length, angle);

	int id_arr[] = { getID(), getID(), getID() };
	AbsLine l = { position->x - x, position->y - y, position->x + x, position->y + y };
	std::shared_ptr<EditableLine> line = std::make_shared<EditableLine>(id_arr[0], id_arr[1], id_arr[2], l);

	lines->emplace(id_arr[0], line);
	nodes->emplace(id_arr[1], line->getNodeA());
	nodes->emplace(id_arr[2], line->getNodeB());
}

void change_recursions_field(tgui::EditBox::Ptr field, int* num_recursions, LineFractal* fractal) {
	tgui::String new_val = field->getText();
	if (new_val.length() == 0) {
		field->setText(std::to_string(*num_recursions));
	}
	else {
		int new_num = new_val.toInt();
		if (new_num != *num_recursions) {
			const int max_val = 20;
			if (new_num > max_val) {
				new_num = max_val;
				field->setText(std::to_string(new_num));
			}
			*num_recursions = new_num;
			fractal->generate(*num_recursions);
		}
	}
}
Editing::Editing(sf::RenderWindow& window) {
	gui = new tgui::Gui{ window };
	setupGUI(window.getSize().x, window.getSize().y);

	// set seed for line creation
	srand(80085);

	// setup base line
	AbsLine l = { editing_frame_center.x-200, editing_frame_center.y, editing_frame_center.x + 200, editing_frame_center.y };
	base_line = std::make_shared<EditableLine>(getID(), getID(), getID(), l);
	base_line->getDrLine()[0].color = sf::Color::Color(80, 80, 80);
	base_line->getDrLine()[1].color = sf::Color::Color(80, 80, 80);
	fractal.setBaseLine(base_line->toAbsLine());
	fillLineColour(fractal.getFractal(), sf::Color::Red);
	fractal.generate(num_recursions);
}

Editing::~Editing() {
	delete gui;
}

void Editing::enter(){
	PRINT("entered editing state");
	//fractal.setOrigin(editing_frame_center.x, editing_frame_center.y);
}

void Editing::run()
{

}

void Editing::exit()
{
}

void Editing::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::Resized) {
		realignGUI(event.size.width, event.size.height);
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Button::Left) {
			left_press_location = mouse_location;
			for (auto& node : nodes) {
				if (node.second->pointIntersection(event.mouseButton.x, event.mouseButton.y)) {
					dragging_nodes.emplace(node.first);
				}
			}
		}
	}
	if (event.type == sf::Event::MouseMoved) {
		bool node_moved = false;
		for (int node_id : dragging_nodes) {
			node_moved = true;
			nodes[node_id]->translate(event.mouseMove.x - mouse_location.x, event.mouseMove.y - mouse_location.y);
		}
		if (node_moved) {
			if (DEBUG) {
				int i = 0;
				for (auto l : lines) {
					LFLine lfl = l.second->toLFLine(base_line->toAbsLine());
					PRINT(i << ": " << lfl.distance << " | " << lfl.angle1 << " | " << lfl.length << " | " << lfl.angle2);
					i++;
				}
			}
			fractalChanged();
		}
		mouse_location.x = event.mouseMove.x;
		mouse_location.y = event.mouseMove.y;
	}
	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Button::Left) {
			dragging_nodes.clear();
			if (mouse_location == left_press_location) {

				// recolour old selected nodes and remove
				for (int sel_node_id : selected_nodes) {
					nodes[sel_node_id]->getDrNode().setOutlineColor(sf::Color::White);
				}
				selected_nodes.clear();

				// select new nodes
				for (auto& node : nodes) {
					if (node.second->pointIntersection(mouse_location.x, mouse_location.y)) {
						node.second->getDrNode().setOutlineColor(sf::Color::Red);
						selected_nodes.emplace(node.first);
					}
				}
				if (DEBUG) {
					PRINT("Seletec ids:");
					for (int i : selected_nodes) {
						PRINT(i);
					}
					PRINT("");
				}
			}
		}
	}
	gui->handleEvent(event);
}

void Editing::drawTo(sf::RenderTarget& surface) const
{
	surface.draw(base_line->getDrLine());
	surface.draw(fractal.getFractal());
	for (auto& line : lines) {
		surface.draw(line.second->getDrLine());
	}
	for (auto& node : nodes) {
		if (selected_nodes.count(node.first) || // if node is selected
		node.second->pointIntersection(mouse_location.x, mouse_location.y)) {
			surface.draw(node.second->getDrNode());
		}
	}
	gui->draw();
}

void Editing::fractalChanged()
{
	std::vector<LFLine> final_lines;
	for (auto& line : lines) {
		LFLine lfl = line.second->toLFLine(base_line->toAbsLine());
		final_lines.push_back(lfl);
	}
	fractal.setDerivedLines(final_lines);
	fractal.generate(num_recursions);
}

void Editing::realignGUI(int window_width, int window_height)
{
	right_panel->setSize(right_panel_width, window_height);
	right_panel->setPosition(window_width - right_panel_width, 0);
	display_button->setPosition(general_padding, window_height - general_padding - display_button_height);
	node_selections->setSize(general_element_width, window_height - (3 * general_padding) - display_button_height);
	editing_frame_center.x = (window_width - right_panel_width) / 2;
	editing_frame_center.y = window_height / 2;
}

void Editing::setupGUI(int window_width, int window_height)
{
	// right panel
	right_panel = tgui::Panel::create();
	gui->add(right_panel);
	right_panel->setInheritedOpacity(0.8);

	// display button
	display_button = tgui::Button::create();
	right_panel->add(display_button);
	display_button->setSize(general_element_width, display_button_height);
	display_button->onClick(cs_viewing);
	display_button->setText("Display fractal");

	// recursions field
	recursions_field = tgui::Panel::create();
	right_panel->add(recursions_field);
	recursions_field->setSize(general_element_width, recursions_field_height);
	recursions_field->setPosition(general_padding, general_padding);

	// recursions input
	recursions_input = tgui::EditBox::create();
	recursions_field->add(recursions_input);
	recursions_input->setSize(tgui::Layout2d{ recursions_input_width, recursions_field_height });
	recursions_input->setPosition(tgui::Layout2d{ general_element_width - recursions_input_width, 0 });
	recursions_input->setText(std::to_string(num_recursions));
	recursions_input->onReturnKeyPress(change_recursions_field, recursions_input, &num_recursions, &fractal);

	// recursions label
	recursions_label = tgui::Label::create();
	recursions_field->add(recursions_label);
	recursions_label->setSize(tgui::Layout2d{ general_element_width - recursions_input_width - general_padding, recursions_field_height });
	recursions_label->setPosition(tgui::Layout2d{ 0, 3 }); // slight adjustment so it lines up with input text
	recursions_label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	recursions_label->setText("Number of recursions");

	// add line button
	add_line_button = tgui::Button::create();
	right_panel->add(add_line_button);
	add_line_button->setSize((general_element_width - general_padding) / 2, add_remove_button_height);
	add_line_button->setPosition(general_padding, recursions_field->getPosition().y + recursions_field_height + (general_padding * 2));
	add_line_button->setText("add line");
	add_line_button->onClick(addLine, &editing_frame_center, &nodes, &lines);

	// remove line button
	remove_line_button = tgui::Button::create();
	right_panel->add(remove_line_button);
	remove_line_button->setSize((general_element_width - general_padding) / 2, add_remove_button_height);
	remove_line_button->setPosition((general_padding * 2) + remove_line_button->getSize().x, recursions_field->getPosition().y + recursions_field_height + (general_padding * 2));
	remove_line_button->setText("remove line");

	// measurements block
	measurements_block = tgui::Panel::create();
	right_panel->add(measurements_block);
	measurements_block->setPosition(general_padding, add_line_button->getPosition().y + add_remove_button_height + general_padding);
	measurements_block->setSize(general_element_width, measurements_block_height);
	
	std::string label_names[] = { "Length", "Angle", "Position" };
	for (int i = 0; i < no_measurement_fields; i++) {
		// measurement inputs
		measurement_inputs[i] = tgui::EditBox::create();
		measurements_block->add(measurement_inputs[i]);
		measurement_inputs[i]->setSize(measurements_input_width, measurements_field_height);
		measurement_inputs[i]->setPosition(general_element_width - measurements_input_width, general_padding + (general_padding + measurements_field_height) * i);
		
		// measurement labels
		measurement_labels[i] = tgui::Label::create();
		measurements_block->add(measurement_labels[i]);
		measurement_labels[i]->setText(label_names[i]);
		measurement_labels[i]->setSize(general_element_width - measurements_input_width - general_padding, measurements_field_height);
		measurement_labels[i]->setPosition(0, general_padding + 6 + (general_padding + measurements_field_height) * i);
	}

	// node selections
	node_selections = tgui::ScrollablePanel::create();
	right_panel->add(node_selections);
	
	node_selections->setPosition(general_padding, measurements_block->getPosition().y + general_padding);

	realignGUI(window_width, window_height);
}
