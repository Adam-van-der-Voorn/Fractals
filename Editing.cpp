#include "Editing.h"
#include "StateMachine.h"
#include "LineFractal.h"
#include "debug_printing.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "vertarrutil.h"
#include "AbsLine.h"
#include "EditableLineNode.h"
#include "SelLineWidget.h"
#include "EditingGUI.h"
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

void Editing::addLine() {
	float length = rand() % 50 + 50; // random length from 50 - 100
	float angle = ((float)(rand() % (3141 * 2))) / 100; // random angle in radians from 0 - ~2pi
	float x = lendirX(length, angle);
	float y = lendirY(length, angle);

	int id_arr[] = { getID(), getID(), getID() };
	AbsLine l = { editing_frame_center.x - x, editing_frame_center.y - y, editing_frame_center.x + x, editing_frame_center.y + y };
	std::shared_ptr<EditableLine> line = std::make_shared<EditableLine>(id_arr[0], id_arr[1], id_arr[2], l);

	lines.emplace(id_arr[0], line);
	nodes.emplace(id_arr[1], line->getNodeA());
	nodes.emplace(id_arr[2], line->getNodeB());
	notifyAll(Event::LINES_CHANGED);
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
	PRINT("originx: " << fractal->getOriginX());
	PRINT("originy: " << fractal->getOriginY());
	PRINT("x1: " << fractal->x1);
	PRINT("x2: " << fractal->x2);
	PRINT("y1: " << fractal->y1);
	PRINT("y2: " << fractal->y2);
}

Editing::Editing(sf::RenderWindow& window) {
	gui = new tgui::Gui{ window };
	setupGUI(window.getSize().x, window.getSize().y);

	// set seed for line creation
	srand(80085);

	// setup base line
	AbsLine l = { editing_frame_center.x-200, editing_frame_center.y, editing_frame_center.x + 200, editing_frame_center.y };
	base_line = std::make_shared<EditableLine>(getID(), getID(), getID(), l);
	fractal.setBaseLine(l);

	gui_2 = std::make_shared<EditingGUI>(this);

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
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (isWithinEditingFrame(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
			if (event.mouseButton.button == sf::Mouse::Button::Left) {
				left_press_location = mouse_framepos;
				for (auto& node : nodes) {
					if (node.second->pointIntersection(event.mouseButton.x, event.mouseButton.y)) {
						dragging_nodes.emplace(node.first);
					}
				}
			}
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		if (isWithinEditingFrame(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {		
			bool node_moved = false;
			for (int node_id : dragging_nodes) {
				node_moved = true;
				nodes[node_id]->translate(event.mouseMove.x - mouse_framepos.x, event.mouseMove.y - mouse_framepos.y);
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
				notifyAll(Event::LINES_CHANGED);
				fractalChanged();
			}
			mouse_framepos.x = event.mouseMove.x;
			mouse_framepos.y = event.mouseMove.y;
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (isWithinEditingFrame(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
			if (event.mouseButton.button == sf::Mouse::Button::Left) {
				dragging_nodes.clear();
				if (mouse_framepos == left_press_location) {
					selected_nodes.clear();

					// select new nodes
					for (auto& node : nodes) {
						if (node.second->pointIntersection(mouse_framepos.x, mouse_framepos.y)) {
							selected_nodes.insert(node.first);
						}
					}
					if (DEBUG) {
						PRINT("Seletec ids:");
						for (int i : selected_nodes) {
							PRINT(i);
						}
						PRINT("");
					}
					notifyAll(Event::SELECTION_CHANGED);
				}
			}
		}
	}
	gui->handleEvent(event);
}

void Editing::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fractal.getFractal());
	
	gui->draw();
	target.draw(*gui_2);
}

const std::unordered_map<int, std::shared_ptr<EditableLineNode>>& Editing::getNodes() const
{
	return nodes;
}

const std::unordered_map<int, std::shared_ptr<EditableLine>>& Editing::getLines() const
{
	return lines;
}

const std::shared_ptr<EditableLine> Editing::getBaseLine() const
{
	return base_line;
}

const std::unordered_set<int>& Editing::getSelectedNodes() const
{
	return selected_nodes;
}

void Editing::addObserver(Observer* observer)
{
	observers.insert(observer);
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

void Editing::notifyAll(Event e) const
{
	for (Observer* observer : observers) {
		observer->onNotify(e);
	}
}

bool Editing::isWithinEditingFrame(sf::Vector2f point) const
{
	if (point.x >= 0 && point.y >= 0 && point.x < editing_frame_size.x && point.y < editing_frame_size.y) {
		return true;
	}
	return false;
}

void Editing::realignGUI(int window_width, int window_height)
{
	right_panel->setSize(right_panel_width, window_height);
	right_panel->setPosition(window_width - right_panel_width, 0);
	display_button->setPosition(general_padding, window_height - general_padding - display_button->getSize().y);
	node_selections->setSize(general_element_width, display_button->getPosition().y - (2 * general_padding) - (measurement_fields[2]->getPosition().y + measurement_fields[2]->getSize().y));
	editing_frame_size.x = (window_width - right_panel_width);
	editing_frame_size.y = window_height;
	editing_frame_center.x = editing_frame_size.x / 2;
	editing_frame_center.y = editing_frame_size.y / 2;
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
	display_button->setSize(general_element_width, 50);
	display_button->onClick(cs_viewing);
	display_button->setText("Display fractal");

	// recursions field
	recursions_field = tgui::Panel::create();
	right_panel->add(recursions_field);
	recursions_field->setSize(general_element_width, 20);
	recursions_field->setPosition(general_padding, general_padding);

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
	recursions_input->setText(std::to_string(num_recursions));
	recursions_input->onReturnKeyPress(change_recursions_field, recursions_input, &num_recursions, &fractal);

	// line actions field
	line_actions_field = tgui::Panel::create();
	right_panel->add(line_actions_field);
	line_actions_field->setSize(general_element_width, 40);
	line_actions_field->setPosition({ general_padding, tgui::bindBottom(recursions_field) + general_padding });

	// add line button
	add_line_button = tgui::Button::create();
	line_actions_field->add(add_line_button);
	add_line_button->setSize((line_actions_field->getSize().x - general_padding) / 2, "100%");
	add_line_button->setPosition(0, 0);
	add_line_button->setText("add line");
	add_line_button->onClick(&Editing::addLine, this);

	// remove line button
	remove_line_button = tgui::Button::create();
	line_actions_field->add(remove_line_button);
	remove_line_button->setSize((general_element_width - general_padding) / 2, "100%");
	remove_line_button->setPosition({ tgui::bindRight(add_line_button) + general_padding, 0 });
	remove_line_button->setText("remove line");
	remove_line_button->onClick(&Editing::enter, this);

	// measurements block
	std::string label_names[] = { "Length", "Angle", "Position" };
	static const int no_measurement_fields = sizeof(measurement_inputs) / sizeof(tgui::EditBox::Ptr);
	for (int i = 0; i < no_measurement_fields; i++) {
		// measurement fields
		measurement_fields[i] = tgui::Panel::create();
		right_panel->add(measurement_fields[i]);
		measurement_fields[i]->setSize(general_element_width, 20);

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
		measurement_inputs[i]->setPosition(general_element_width - measurement_inputs[i]->getSize().x, 0);
	}
	measurement_fields[0]->setPosition(general_padding, tgui::bindBottom(line_actions_field) + general_padding);
	measurement_fields[1]->setPosition(general_padding, tgui::bindBottom(measurement_fields[0]) + general_padding);
	measurement_fields[2]->setPosition(general_padding, tgui::bindBottom(measurement_fields[1]) + general_padding);


	// node selections
	node_selections = tgui::ScrollablePanel::create();
	right_panel->add(node_selections);
	node_selections->setPosition({ general_padding, tgui::bindBottom(measurement_fields[2]) + general_padding });
	

	realignGUI(window_width, window_height);
}
