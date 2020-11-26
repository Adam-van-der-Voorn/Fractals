#include "SelLineWidget.h"
#include "EditableLineNode.h"
#include "Editing.h"
#include "EditableLine.h"
#include "vecutil.h"
#include <unordered_map>

class Editing;

SelLineWidget::SelLineWidget(Editing* editing, int node_id, float width, float height)
	: editing(editing), node_id(node_id)
{
	setSize(width, height);
	setWidgetName(std::to_string(node_id));
	add(select_button);
	select_button->setSize("100%", "100%");
	select_button->onClick(&Editing::selectOnlyHoveredNode, editing);
	select_button->onMouseEnter(&Editing::setHoveredNode, editing, node_id);
	select_button->onMouseLeave(&Editing::setHoveredNode, editing, -1);
	add(icon);
	constexpr float icon_padding = 3;
	icon->setSize(height - icon_padding * 2, height - icon_padding * 2);
	icon->setPosition(icon_padding, icon_padding);
	redrawIcon();
	editing->addObserver(this);
}

SelLineWidget::~SelLineWidget() {
	editing->removeObserver(this);
}

SelLineWidget::SelLineWidget(const SelLineWidget& copy_from)
{
	editing = copy_from.editing;
	node_id = copy_from.node_id;
	setPosition({ copy_from.getPosition().x, copy_from.getPosition().y }); // init list is neccesarcy bc of tgui quirks
	setSize({ copy_from.getSize().x, copy_from.getSize().y });
	setWidgetName(std::to_string(node_id));
	add(select_button);
	select_button->setSize("100%", "100%");
	select_button->onClick(&Editing::selectOnlyHoveredNode, editing);
	select_button->onMouseEnter(&Editing::setHoveredNode, editing, node_id);
	select_button->onMouseLeave(&Editing::setHoveredNode, editing, -1);
	add(icon);
	constexpr float icon_padding = 3;
	icon->setSize(copy_from.getSize().y - icon_padding * 2, copy_from.getSize().y - icon_padding * 2);
	icon->setPosition(icon_padding, icon_padding);
	redrawIcon();
	editing->addObserver(this);
}

void SelLineWidget::onNotify(int event_num)
{
	auto event = static_cast<Editing::Event>(event_num);
	switch (event)
	{
	case Editing::Event::LINES_CHANGED:
		redrawIcon();
		break;
	}
}

void SelLineWidget::redrawIcon() {
	icon->clear();
	EditableLineNode* node = editing->getNodes().at(node_id).get();
	double line_angle = lineAngle(node->getLine()->toAbsLine());

	// line
	if (node->isFront()) {
		line_angle += m_pi;
	}
	sf::VertexArray line_shape = sf::VertexArray(sf::Lines, 2);
	line_shape[0].position = sf::Vector2f(icon->getSize().x / 2, icon->getSize().y / 2);
	line_shape[1].position = sf::Vector2f(lendirX(999, line_angle), lendirY(999, line_angle));
	icon->draw(line_shape);

	// node
	sf::CircleShape node_shape(icon->getSize().x / 3);
	node_shape.setOrigin(node_shape.getRadius(), node_shape.getRadius());
	node_shape.setPosition(icon->getSize().x / 2, icon->getSize().y / 2);
	node_shape.setFillColor(sf::Color::Transparent);
	node_shape.setOutlineColor(sf::Color::White);
	node_shape.setOutlineThickness(1.0f);
	icon->draw(node_shape);

	// arrowhead
	if (node->isFront()) {
		auto arrowhead_shape = sf::VertexArray(sf::LineStrip, 3);
		arrowhead_shape[1].position = sf::Vector2f(icon->getSize().x / 2, icon->getSize().y / 2);
		arrowhead_shape[0].position.x = arrowhead_shape[1].position.x + lendirX(node_shape.getRadius(), line_angle + m_pi4);
		arrowhead_shape[0].position.y = arrowhead_shape[1].position.y + lendirY(node_shape.getRadius(), line_angle + m_pi4);
		arrowhead_shape[2].position.x = arrowhead_shape[1].position.x + lendirX(node_shape.getRadius(), line_angle - m_pi4);
		arrowhead_shape[2].position.y = arrowhead_shape[1].position.y + lendirY(node_shape.getRadius(), line_angle - m_pi4);
		icon->draw(arrowhead_shape);
	}
	icon->display();
}

tgui::Widget::Ptr SelLineWidget::clone() const
{
	auto tmp = std::make_shared<SelLineWidget>(*this);
	return tmp;
}
