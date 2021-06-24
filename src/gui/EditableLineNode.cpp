#include "EditableLineNode.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "LFLine.h"
#include <cassert>

EditableLineNode::EditableLineNode(int id, Vec2 pos, bool is_front, EditableLine* line) : id(id), pos(pos), is_front(is_front), line(line)
{
}

bool EditableLineNode::pointIntersection(Vec2 point) const
{
	if (distanceBetweenAB(point.x, point.y, pos.x, pos.y) <= NODE_RADIUS) {
		return true;
	}
	return false;
}

void EditableLineNode::translate(Vec2 translation)
{
	pos += translation;
}

void EditableLineNode::setPosition(Vec2 position)
{
	pos = position;
}

void EditableLineNode::setLength(double length)
{
	double line_angle = getAngle();
	getOtherNode()->setPosition({ pos.x + lendirX(length, line_angle), pos.y + lendirY(length, line_angle) });
}

void EditableLineNode::setAngle(double angle)
{
	double line_length = getLength();
	getOtherNode()->setPosition({ pos.x + lendirX(line_length, angle), pos.y + lendirY(line_length, angle) });
}

Vec2 EditableLineNode::getPosition() const
{
	return pos;
}

double EditableLineNode::getLength() const
{
	return lineLength(getLine()->toAbsLine());
}

double EditableLineNode::getAngle() const
{
	EditableLineNode* other = getOtherNode();
	return lineAngle({pos, other->getPosition()});
}

bool EditableLineNode::isFront() const
{
	return is_front;
}
EditableLine* EditableLineNode::getLine() const
{
	return line;
}
EditableLineNode* EditableLineNode::getOtherNode() const
{
	if (isFront()) {
		return line->getBackNode();
	}
	else {
		return line->getFrontNode();
	}
}
int EditableLineNode::getID() const
{
	return id;
}
