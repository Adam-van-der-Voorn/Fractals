#include "SelLineWidget.h"
#include "EditableLineNode.h"
#include "Editing.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "debug_printing.h"
#include "degrad.h"
#include <unordered_map>
#include <memory>

class Editing;

SelLineWidget::SelLineWidget(Editing* editing, int node_id, float width)
	: editing(editing), node_id(node_id)
{
	setSize({ width, 140 });
	init();
}

SelLineWidget::~SelLineWidget() {
	editing->removeObserver(this);
}

SelLineWidget::SelLineWidget(const SelLineWidget& copy_from)
	: editing(copy_from.editing), node_id(copy_from.node_id)
{
	setPosition({ copy_from.getPosition().x, copy_from.getPosition().y }); // init list is neccesarcy bc of tgui quirks
	setSize({ copy_from.getSize().x, copy_from.getSize().y });
	init();
}

void SelLineWidget::init()
{
	setWidgetName(std::to_string(node_id));
	//temp bckg
	add(temp_background);
	temp_background->setSize("100%", "100%");
	temp_background->clear(tgui::Color::Yellow);

	// icon
	icon = tgui::Canvas::create();
	add(icon);
	constexpr float icon_padding = 3;
	icon->setSize(50 - icon_padding * 2, 50 - icon_padding * 2);
	icon->setPosition(icon_padding, icon_padding);
	redrawIcon();

	// selection button
	select_button = tgui::Button::create();
	add(select_button);
	select_button->setSize(40, 40);
	select_button->setPosition(tgui::bindRight(icon) + padding, 6);
	select_button->onClick(&Editing::selectOnlyHoveredNode, editing);
	select_button->onMouseEnter(&Editing::setHoveredNode, editing, node_id);
	select_button->onMouseLeave(&Editing::setHoveredNode, editing, -1);
	select_button->setText("sel");

	// measurements block
	constexpr float input_height = 22;
	constexpr float label_height = 18;
	constexpr float label_input_padding = 1;
	const float field_width = (getSize().x - (padding * 3)) / 2;

	// x position
	auto xpos_label = tgui::Label::create();
	add(xpos_label);
	xpos_label->setPosition(padding, tgui::bindBottom(icon) + padding);
	xpos_label->setSize({ field_width, label_height });
	xpos_label->setText("xpos");

	xpos_input = std::make_shared<NumFieldExt>(editing->getValClipboard());
	add(xpos_input);
	xpos_input->setPosition(padding, tgui::bindBottom(xpos_label) + label_input_padding);
	xpos_input->setSize({ field_width, input_height });
	xpos_input->setVal(editing->getNodes().at(node_id)->getPosition().x);
	xpos_input->setMaximumCharacters(7);

	// y position
	auto ypos_label = tgui::Label::create();
	add(ypos_label);
	ypos_label->setPosition(tgui::bindRight(xpos_input) + padding, tgui::bindBottom(icon) + padding);
	ypos_label->setSize({ field_width, label_height });
	ypos_label->setText("ypos");

	ypos_input = std::make_shared<NumFieldExt>(editing->getValClipboard());
	add(ypos_input);
	ypos_input->setPosition(tgui::bindRight(xpos_input) + padding, tgui::bindBottom(ypos_label) + label_input_padding);
	ypos_input->setSize({ field_width, input_height });
	ypos_input->setVal(editing->getNodes().at(node_id)->getPosition().y);
	ypos_input->setMaximumCharacters(7);

	// direction of line
	auto dir_label = tgui::Label::create();
	add(dir_label);
	dir_label->setPosition(padding, tgui::bindBottom(xpos_input) + padding);
	dir_label->setSize({ field_width, label_height });
	dir_label->setText("angle");

	dir_input = std::make_shared<NumFieldExt>(editing->getValClipboard());
	add(dir_input);
	dir_input->setPosition(padding, tgui::bindBottom(dir_label) + label_input_padding);
	dir_input->setSize({ field_width, input_height });
	dir_input->setVal(toDeg(editing->getNodes().at(node_id)->getAngle()));
	dir_input->setMaximumCharacters(7);

	// length of line
	auto len_label = tgui::Label::create();
	add(len_label);
	len_label->setPosition(tgui::bindRight(dir_input) + padding, tgui::bindBottom(xpos_input) + padding);
	len_label->setSize({ field_width, label_height });
	len_label->setText("length");

	len_input = std::make_shared<NumFieldExt>(editing->getValClipboard());
	add(len_input);
	len_input->setPosition(tgui::bindRight(dir_input) + padding, tgui::bindBottom(len_label) + padding);
	len_input->setSize({ field_width, input_height });
	len_input->setVal(editing->getNodes().at(node_id)->getLength());
	len_input->setMaximumCharacters(7);

	editing->addObserver(this);
	xpos_input->addObserver(this);
	ypos_input->addObserver(this);
	dir_input->addObserver(this);
	len_input->addObserver(this);
}

void SelLineWidget::onNotify(Editing* e, int event_num)
{
	auto event = static_cast<Editing::Event>(event_num);
	switch (event)
	{
	case Editing::Event::LINES_CHANGED:
		redrawIcon();
		xpos_input->setVal(editing->getNodes().at(node_id)->getPosition().x);
		ypos_input->setVal(editing->getNodes().at(node_id)->getPosition().y);
		dir_input->setVal(toDeg(editing->getNodes().at(node_id)->getAngle()));
		len_input->setVal(editing->getNodes().at(node_id)->getLength());
		break;
	}
}

void SelLineWidget::onNotify(NumFieldExt* field, int event_num)
{
	if (*field == *xpos_input.get() || *field == *ypos_input.get()) {
		editing->setNodePosition(node_id, { xpos_input->getVal(), ypos_input->getVal() });
	}
	else if (*field == *len_input.get()) {
		editing->setNodeLength(node_id, len_input->getVal());
	}
	else {
		assert(*field == *dir_input.get() && "field not == to anything");
		editing->setNodeAngle(node_id, toRad(dir_input->getVal()));
	}
}

void SelLineWidget::redrawIcon() {
	icon->clear();
	EditableLineNode* node = editing->getNodes().at(node_id);
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
