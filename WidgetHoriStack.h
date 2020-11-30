#pragma once
#include <TGUI/Widgets/Panel.hpp>
class WidgetHoriStack : public tgui::Panel
{
public:
	WidgetHoriStack();

	/**
	swaps the widget stack with the given stack.
	Widgets that are aleady in the old stack retain there position, and new widgets are appened at the bottom.
	\param new_widgets the new stack
	**/
	void swapStack(std::vector<tgui::Widget::Ptr>& new_widgets);

	/**
	sets the left and top margins for the widgets
	\param left the left margin
	\param top the top margin
	**/
	void setMargins(float left, float top);

	/**
	clears the stack
	**/
	void clear();
private:
	void reconstructWidgetStack();
	std::vector<tgui::Widget::Ptr> widgets;
	float top_margin, left_margin;
};

