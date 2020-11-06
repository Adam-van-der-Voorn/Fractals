#include "WidgetHoriStack.h"

WidgetHoriStack::WidgetHoriStack(tgui::Panel::Ptr panel) : panel(panel)
{
}

tgui::Panel::Ptr WidgetHoriStack::getPanel()
{
	return panel;
}

void WidgetHoriStack::appendWidget(tgui::Widget::Ptr widget, float xpos, float buffer)
{
	panel->add(widget);
	if (widgets.size() == 0) {
		widget->setPosition(xpos, buffer);
	}
	else {
		widget->setPosition(xpos, tgui::bindBottom(widgets.back()) + buffer);
	}
	widgets.push_back(widget);
}

void WidgetHoriStack::insertWidget(tgui::Widget::Ptr widget, float xpos, float buffer, int index)
{
}

void WidgetHoriStack::removeWidget(int index)
{
}

void WidgetHoriStack::clearAllWidgets()
{
	panel->removeAllWidgets();
	widgets.clear();
}
