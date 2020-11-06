#pragma once
#include <TGUI/Widgets/Panel.hpp>
class WidgetHoriStack
{
public:
	WidgetHoriStack(tgui::Panel::Ptr panel);
	tgui::Panel::Ptr getPanel();
	void appendWidget(tgui::Widget::Ptr widget, float xpos, float buffer);
	void insertWidget(tgui::Widget::Ptr widget, float xpos, float buffer, int index);
	void removeWidget(int index);
	void clearAllWidgets();
private:
	tgui::Panel::Ptr panel;
	std::vector<tgui::Widget::Ptr> widgets;
};

