#pragma once
#include <TGUI/Widgets/Panel.hpp>
class WidgetHoriStack
{
public:
	WidgetHoriStack(tgui::Panel::Ptr panel);
	tgui::Panel::Ptr getPanel();
	void swapStack(std::vector<tgui::Widget::Ptr>& new_widgets);
	void setMargins(float left, float top);
	void clear();
private:
	void reconstructPanel();
	tgui::Panel::Ptr panel;
	std::vector<tgui::Widget::Ptr> widgets;
	float top_margin, left_margin;
};

