     #pragma once
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include "Subject.h"
/**
height is set, only field changes with width
**/
class NumFieldExt : public tgui::Panel, public Subject<NumFieldExt>
{
public:
	typedef std::shared_ptr<NumFieldExt> Ptr; //!< Shared widget pointer
	typedef std::shared_ptr<const NumFieldExt> ConstPtr; //!< Shared constant widget pointer

	NumFieldExt(double* clipboard);

	/**
	\return the actual value of the input field
	**/
	double getVal() const;

	/**
	Sets the actual value of the input field
	\param val the value to set it to
	**/
	void setVal(double val);

	/**
	Sets the width of the widget
	\param the width of the widget
	**/
	void setSize(const tgui::Layout2d& size) override;

	/**
	Sets the maximum amount of characters. set to 0 for unlimited.
	\param max the amount of chars
	**/
	void setMaximumCharacters(unsigned int max);

	bool operator == (NumFieldExt) const;

private:
	void init();
	void copy();
	void paste();

	/**
	should be called when the field has been unfocused or return has been pressed.
	if the field has changed, the actual value will update.
	if the field has not changed, the value will not either.
	**/
	void confirmFieldChange();

	// what the field says. Unlike the widget, this is not udated until the change to the field is confirmed.
	tgui::String field_value = "";

	// the actual value of the field, more accurate than the string in the field itself
	double actual_value;

	const float padding = 3;

	double* clipboard;

	tgui::EditBox::Ptr input = tgui::EditBox::create();
	tgui::Button::Ptr copy_button = tgui::Button::create();
	tgui::Button::Ptr paste_button = tgui::Button::create();
};
