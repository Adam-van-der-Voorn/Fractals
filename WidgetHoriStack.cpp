#include "WidgetHoriStack.h"

WidgetHoriStack::WidgetHoriStack()
{
}

void WidgetHoriStack::swapStack(std::vector<tgui::Widget::Ptr>& new_widgets)
{
	// remove any widgets that are NOT in new_widgets but are in widgets
	for (size_t i = 0; i < widgets.size(); i++) {
		bool is_new_widget = false;
		for (tgui::Widget::Ptr new_widget : new_widgets) {
			if (widgets[i]->getWidgetName() == new_widget->getWidgetName()) {
				is_new_widget = true; // old widget is found in new widgets, so dont remove
				break;
			}
		}
		if (!is_new_widget) {
			widgets.erase(widgets.begin() + i);
			i--;
		}

	}

	// check if any of the widgets in new_widgets are in widgets. if so, remove them from new_widgets
	for (size_t i = 0; i < new_widgets.size(); i++) {
		for (tgui::Widget::Ptr widget : widgets) {
			if (new_widgets[i]->getWidgetName() == widget->getWidgetName()) {
				new_widgets.erase(new_widgets.begin() + i);
				i--;
			}
		}
	}

	// add the remaining new_widgets to the list
	for (tgui::Widget::Ptr new_widget : new_widgets) {
		widgets.emplace_back(new_widget);
	}

	reconstructWidgetStack();
	
}

void WidgetHoriStack::reconstructWidgetStack()
{
	removeAllWidgets();
	if (widgets.size() > 0) {
		add(widgets[0]);
		widgets[0]->setPosition(left_margin, 0);
		for (size_t i = 1; i < widgets.size(); i++) {
			add(widgets[i]);
			widgets[i]->setPosition(left_margin, tgui::bindBottom(widgets[i-1]) + top_margin);
		}
	}
}

void WidgetHoriStack::setMargins(float left, float top)
{
	left_margin = left;
	top_margin = top;
	reconstructWidgetStack();
}

void WidgetHoriStack::clear()
{
	removeAllWidgets();
	widgets.clear();
}
