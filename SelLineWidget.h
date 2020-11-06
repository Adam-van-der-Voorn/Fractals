#pragma once
#include <TGUI/Widgets/Panel.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <unordered_map>

class EditableLineNode;

class SelLineWidget
{
public:
	SelLineWidget(std::unordered_map<int, std::shared_ptr<EditableLineNode>>& nodes, std::unordered_set<int>& selected_nodes,
		int node_id, float width, float height, float xpos = 0, float ypos = 0);
	tgui::Panel::Ptr getPanel();

private:
	std::unordered_map<int, std::shared_ptr<EditableLineNode>> nodes;
	std::unordered_set<int> selected_nodes;
	int node_id;
	tgui::Panel::Ptr panel;
	tgui::Button::Ptr select_button;
	sf::CircleShape dr_circle;
	sf::VertexArray dr_line;
};

