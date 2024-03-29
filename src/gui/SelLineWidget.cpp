#include "SelLineWidget.h"

#include "EditableLineNode.h"
#include "Editing.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "debug_printing.h"
#include "degrad.h"
#include "FrameState.h"
#include <unordered_map>
#include <memory>

class Editing;

SelLineWidget::SelLineWidget(Editing* editing, NodeID node_id, float width)
	: editing(editing), frame(editing->getFrame()), node_id(node_id)
{
	setSize({ width, 170 });
	init();
	remapGuiElements();
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
	remapGuiElements();
}

SelLineWidget& SelLineWidget::operator =(const SelLineWidget& w)
{
	node_id = w.node_id;
	editing = w.editing;
	setPosition({ w.getPosition().x, w.getPosition().y }); // init list is neccesarcy bc of tgui quirks
	setSize({ w.getSize().x, w.getSize().y });
	remapGuiElements();
	return *this;
}


void SelLineWidget::init()
{
	add(temp_background, "temp_background");

	icon = tgui::Canvas::create();
	add(icon, "icon");
	
	select_button = tgui::Button::create();
	add(select_button, "select_button");
	
	auto xpos_label = tgui::Label::create();
	add(xpos_label, "xpos_label");
	
	xpos_input = std::make_shared<NumFieldExt>(editing->getValClipboard());
	add(xpos_input, "xpos_input");
	
	auto ypos_label = tgui::Label::create();
	add(ypos_label, "ypos_label");
	
	ypos_input = std::make_shared<NumFieldExt>(editing->getValClipboard());
	add(ypos_input, "ypos_input");
	
	auto dir_label = tgui::Label::create();
	add(dir_label, "dir_label");
	
	dir_input = std::make_shared<NumFieldExt>(editing->getValClipboard());
	add(dir_input, "dir_input");
	
	auto len_label = tgui::Label::create();
	add(len_label, "len_label");

	len_input = std::make_shared<NumFieldExt>(editing->getValClipboard());
	add(len_input, "len_input");

	auto rec_label = tgui::Label::create();
	add(rec_label, "rec_label");

	rec_checkbox = tgui::CheckBox::create();
	add(rec_checkbox, "rec_checkbox");

	editing->addObserver(this);
	xpos_input->addObserver(this);
	ypos_input->addObserver(this);
	dir_input->addObserver(this);
	len_input->addObserver(this);
}

void SelLineWidget::remapGuiElements()
{
	setWidgetName(node_id.toString());

	//temp bckg
	temp_background->setSize("100%", "100%");
	temp_background->clear(tgui::Color::Yellow);

	// icon
	constexpr float icon_padding = 3;
	icon->setSize(50 - icon_padding * 2, 50 - icon_padding * 2);
	icon->setPosition(icon_padding, icon_padding);
	redrawIcon();

	// selection button
	select_button->setSize({131, 40});
	select_button->setPosition(tgui::bindRight(icon) + padding, 6);
	select_button->onClick(&Editing::selectOnlyHoveredNode, editing);
	select_button->onMouseEnter(&Editing::setHoveredNode, editing, node_id);
	select_button->onMouseLeave(&Editing::setHoveredNode, editing, NodeID::nonexistent());
	select_button->setText("Select");

	// measurements block
	constexpr float input_height = 22;
	constexpr float label_height = 18;
	constexpr float label_input_padding = 1;
	const float field_width = (getSize().x - (padding * 3)) / 2;

	// x position field
	auto xpos_label = get<tgui::Label>("xpos_label");
	xpos_label->setPosition(padding, tgui::bindBottom(icon) + padding);
	xpos_label->setSize({ field_width, label_height });
	xpos_label->setText("xpos");

	xpos_input->setPosition(padding, tgui::bindBottom(xpos_label) + label_input_padding);
	xpos_input->setSize({ field_width, input_height });
	xpos_input->setVal(frame->getNode(node_id)->getPosition().x);
	xpos_input->setMaximumCharacters(7);

	// y position field
	auto ypos_label = get<tgui::Label>("ypos_label");
	ypos_label->setPosition(tgui::bindRight(xpos_input) + padding, tgui::bindBottom(icon) + padding);
	ypos_label->setSize({ field_width, label_height });
	ypos_label->setText("ypos");

	ypos_input->setPosition(tgui::bindRight(xpos_input) + padding, tgui::bindBottom(ypos_label) + label_input_padding);
	ypos_input->setSize({ field_width, input_height });
	ypos_input->setVal(frame->getNode(node_id)->getPosition().y);
	ypos_input->setMaximumCharacters(7);

	// direction of line field
	auto dir_label = get<tgui::Label>("dir_label");
	dir_label->setPosition(padding, tgui::bindBottom(xpos_input) + padding);
	dir_label->setSize({ field_width, label_height });
	dir_label->setText("angle");

	double line_angle = frame->getLine(node_id.lineID()).getAngle(node_id.isFront());
	dir_input->setPosition(padding, tgui::bindBottom(dir_label) + label_input_padding);
	dir_input->setSize({ field_width, input_height });
	dir_input->setVal(toDeg(line_angle));
	dir_input->setMaximumCharacters(7);

	// length of line field
	auto len_label = get<tgui::Label>("len_label");
	add(len_label);
	len_label->setPosition(tgui::bindRight(dir_input) + padding, tgui::bindBottom(xpos_input) + padding);
	len_label->setSize({ field_width, label_height });
	len_label->setText("length");

	len_input->setPosition(tgui::bindRight(dir_input) + padding, tgui::bindBottom(len_label) + padding);
	len_input->setSize({ field_width, input_height });
	len_input->setVal(frame->getLine(node_id.lineID()).getLength());
	len_input->setMaximumCharacters(7);

	// recursion checkbox
	auto rec_label = get<tgui::Label>("rec_label");
	rec_label->setPosition(padding, tgui::bindBottom(dir_input) + padding);
	
	rec_checkbox->setPosition(tgui::bindRight(rec_label) + padding, tgui::bindBottom(dir_input) + padding);
	rec_checkbox->setChecked(frame->getLine(node_id.lineID()).isRecursive());
	rec_checkbox->onCheck(&FrameState::setLineRecursiveness, frame, node_id.lineID(), true);
	rec_checkbox->onUncheck(&FrameState::setLineRecursiveness, frame, node_id.lineID(), false);

	rec_label->setSize(getSize().x - rec_checkbox->getSize().x - (padding * 3), label_height);
	rec_label->setText("is recursive");

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
		xpos_input->setVal(frame->getNode(node_id)->getPosition().x);
		ypos_input->setVal(frame->getNode(node_id)->getPosition().y);
		dir_input->setVal(toDeg(frame->getLine(node_id.lineID()).getAngle(node_id.isFront())));
		len_input->setVal(frame->getLine(node_id.lineID()).getLength());
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
	temp_background->clear(tgui::Color::Yellow);
	icon->clear();
	const EditableLineNode* node = frame->getNode(node_id);
	double line_angle = frame->getLine(node_id.lineID()).getAngle(node_id.isFront());

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

// overridden from widget
tgui::Widget::Ptr SelLineWidget::clone() const
{
	auto tmp = std::make_shared<SelLineWidget>(*this);
	return tmp;
}
