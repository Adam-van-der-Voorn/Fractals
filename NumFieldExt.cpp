#include "NumFieldExt.h"
#include <cstdlib>
#include <string>


NumFieldExt::NumFieldExt(tgui::String label_text, double* clipboard)
	: clipboard(clipboard)
{
	init();
	label->setText(label_text);
}

double NumFieldExt::getVal() const
{
	return actual_value;
}

void NumFieldExt::setVal(double val)
{
	actual_value = val;
	field_value = std::to_string(val);
	input->setText(field_value);
}

void NumFieldExt::setInputBoxWidth(float width)
{
	input->setWidth(width);
	label->setWidth(getSize().x - width - copy_button->getSize().x - paste_button->getSize().x - (padding * 4));
}

void NumFieldExt::setLabelWidth(float width)
{
	label->setWidth(width);
	input->setWidth(getSize().x - width - copy_button->getSize().x - paste_button->getSize().x - (padding * 4));
}

void NumFieldExt::setMaximumCharacters(unsigned int max)
{
	input->setMaximumCharacters(max);
}

bool NumFieldExt::operator==(NumFieldExt other) const
{
	if (other.getVal() == getVal() &&
		other.getPosition() == getPosition() &&
		other.getSize() == getSize()) {
		return true;
	}
	return false;
}

void NumFieldExt::init()
{
	setHeight(20);

	// label
	add(label);
	label->setSize(30, "100%");

	// input
	add(input);
	input->setHeight(getSize().y);
	input->setWidth(70);
	input->setPosition(tgui::bindRight(label) + padding, 0);
	input->onReturnOrUnfocus(&NumFieldExt::confirmFieldChange, this);

	// copy
	add(copy_button);
	copy_button->setHeight(getSize().y);
	copy_button->setWidth(12);
	copy_button->setText("c");
	copy_button->onClick(&NumFieldExt::copy, this);
	copy_button->setPosition(tgui::bindRight(input) + padding, 0);


	// paste
	add(paste_button);
	paste_button->setHeight(getSize().y);
	paste_button->setWidth(12);
	paste_button->setText("p");
	paste_button->onClick(&NumFieldExt::paste, this);
	paste_button->setPosition(tgui::bindRight(copy_button) + padding, 0);
}

void NumFieldExt::copy()
{
	*clipboard == actual_value;
}

void NumFieldExt::paste()
{
	setVal(*clipboard);
}

void NumFieldExt::confirmFieldChange()
{
	tgui::String new_input = input->getText();
	if (new_input != field_value) {
		field_value = new_input;
		actual_value = stod(new_input.toAnsiString());
		notifyAll(0);
	}
}
