#include "EditableLine.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "vecutil.h"
#include "LFLine.h"
#include "EditableLineNode.h"
#include <memory>
#include <cassert>

EditableLine::EditableLine(int id, const AbsLine& line) :
id(id),
back_node(EditableLineNode(line.back, false)),
front_node(EditableLineNode(line.head, true))
{
}

void EditableLine::setNodePos(bool is_front, const Vec2& new_pos) 
{
	node(is_front)->setPosition(new_pos);
}

void EditableLine::translateNode(bool is_front, const Vec2& translation) 
{
	EditableLineNode* n = node(is_front);
	const Vec2& old_pos = n->getPosition();
	n->setPosition(old_pos + translation);
}

const EditableLineNode* EditableLine::getNode(bool is_front) const {
	if (is_front) {
		return &front_node;
	}
	else {
		return &back_node;
	}
}

const std::vector<const EditableLineNode*> EditableLine::getNodes() const
{
	// cannot convert 'const EditableLineNode' to 'EditableLineNode*' in initialization
	const EditableLineNode* back_node_ptr = &back_node;
	const EditableLineNode* front_node_ptr = &front_node;
	return {back_node_ptr, front_node_ptr};
}

EditableLineNode* EditableLine::node(bool is_front) {
	if (is_front) {
		return &front_node;
	}
	else {
		return &back_node;
	}
}

double EditableLine::getAngle(bool from_front) const
{
	const EditableLineNode* node = getNode(from_front);
	const EditableLineNode* other = getNode(!from_front);
	return lineAngle({node->getPosition(), other->getPosition()});
}

double EditableLine::getLength() const
{
	return toAbsLine().length();
}

void EditableLine::setRecursive(bool b)
{
	recursive = b;
}

bool EditableLine::isRecursive() const
{
	return recursive;
}

LFLine EditableLine::toLFLine(AbsLine base_line) const
{
	double base_line_len = lineLength(base_line);
	double base_line_angle = lineAngle(base_line);
	double distance = distanceBetweenAB(base_line.back.x, base_line.back.y, back_node.getPosition().x, back_node.getPosition().y) / base_line_len;
	double angle1 = angleBetweenAB(base_line.back.x, base_line.back.y, back_node.getPosition().x, back_node.getPosition().y) - base_line_angle;
	double length = distanceBetweenAB(back_node.getPosition().x, back_node.getPosition().y, front_node.getPosition().x, front_node.getPosition().y) / base_line_len;
	double angle2 = angleBetweenAB(back_node.getPosition().x, back_node.getPosition().y, front_node.getPosition().x, front_node.getPosition().y) - base_line_angle;
	return { distance, angle1, length, angle2, recursive};
}

AbsLine EditableLine::toAbsLine() const
{
	return { back_node.getPosition(), front_node.getPosition() };
}

int EditableLine::getID() const
{
	return id;
}

