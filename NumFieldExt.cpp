#include "NumFieldExt.h"
#include <cstdlib>
#include <string>


NumFieldExt::NumFieldExt(double* clipboard)
	: clipboard(clipboard)
{
	init();
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

void NumFieldExt::setSize(const tgui::Layout2d& size)
{
	Panel::setSize(size);
	input->setSize({ size.x - copy_button->getSize().x - paste_button->getSize().x - (padding * 2), size.y });
	copy_button->setHeight(size.y);
	paste_button->setHeight(size.y);
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

	// input
	add(input);
	input->setHeight(getSize().y);
	input->setPosition(0, 0);
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

	setWidth(100); // has to be last to adjust the input width based on the amount of room left
}

void NumFieldExt::copy()
{
	*clipboard = actual_value;
}

void NumFieldExt::paste()
{
	setVal(*clipboard);
	notifyAll(0);
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
