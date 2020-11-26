#pragma once
#include "SelLineWidget.h"
#include "Editing.h"
#include "Observer.h"
#include <TGUI/Widgets/Panel.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Canvas.hpp>
#include <unordered_map>

class EditableLineNode;

class SelLineWidget : public Observer, public tgui::Panel
{
public:
	SelLineWidget(Editing* editing, int node_id, float width, float height);
	~SelLineWidget();
	SelLineWidget(const SelLineWidget&); 
	void onNotify(int event_num);
	void redrawIcon();

	// Inherited via Widget
	tgui::Widget::Ptr clone() const override;

private:
	int node_id;
	Editing* editing;
	tgui::Button::Ptr select_button = tgui::Button::create();
	tgui::Canvas::Ptr icon = tgui::Canvas::create();
};

