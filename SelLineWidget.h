#pragma once
#include "SelLineWidget.h"
#include "Editing.h"
#include <TGUI/Widgets/Panel.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <unordered_map>

class EditableLineNode;

class SelLineWidget
{
public:
	SelLineWidget(Editing* editing, int node_id, float width, float height, float xpos = 0, float ypos = 0);
	tgui::Panel::Ptr getPanel();

private:
	int node_id;
	tgui::Panel::Ptr panel = tgui::Panel::create();
	tgui::Button::Ptr select_button = tgui::Button::create();
};

