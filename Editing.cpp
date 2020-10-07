#include "Editing.h"
#include "StateMachine.h"
#include "LineFractal.h"
#include "debug_printing.h"
#include "EditableLine.h"
#include "vecutil.h"
#include <cstdlib>

extern StateMachine state_machine;
extern LineFractal fractal;

void cs_viewing() {
	state_machine.changeState("viewing");
}

int getID() {
	static int i = 0;
	return i++;
}

void addLine(std::unordered_map<int, EditableLine>* lines) {
	float length = rand() % 50 + 50; // random length from 50 - 100
	float angle = ((float)(rand() % (3141 * 2))) / 100; // random angle in radians from 0 - ~2pi
	float x = lendir_x(length, angle);
	float y = lendir_y(length, angle);

	int id = getID();
	lines->emplace(id, EditableLine(id, -x, -y, x, y));
}

Editing::Editing(sf::RenderWindow* window) {
	gui = new tgui::Gui{ *window };
	setupGUI(window->getSize().x, window->getSize().y);
	srand(80085);
}

Editing::~Editing() {
	delete gui;
}

void Editing::enter(){
	PRINT("entered editing state");
	fractal.setOrigin(editing_frame_center.x, editing_frame_center.y);
	fractal.generate(5);
}

void Editing::run()
{
}

void Editing::exit()
{
}

void Editing::handleEvent(sf::Event & event)
{
	if (event.type == sf::Event::Resized) {
		realignGUI(event.size.width, event.size.height);
	}
	gui->handleEvent(event);
}

void Editing::draw(sf::RenderWindow* window) const
{
	gui->draw();
	for (auto l : lines) {
		l.second.draw(window);
	}

	window->draw(*fractal.getFractal());
}

void Editing::realignGUI(int window_width, int window_height)
{
	right_panel->setSize(right_panel_width, window_height);
	right_panel->setPosition(window_width - right_panel_width, 0);
	display_button->setPosition(general_padding, window_height - general_padding - display_button_height);
	editing_frame_center.x = (window_width - right_panel_width) / 2;
	editing_frame_center.y = window_height / 2;
}

void Editing::setupGUI(int window_width, int window_height)
{
	// right panel
	right_panel = tgui::Panel::create();
	gui->add(right_panel);
	right_panel->setInheritedOpacity(1);

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
	//recursions_input->onTextChange(func);

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
	add_line_button->onClick(addLine, &lines);

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

	realignGUI(window_width, window_height);
}
