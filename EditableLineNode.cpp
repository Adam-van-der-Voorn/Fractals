#include "EditableLineNode.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "LFLine.h"

EditableLineNode::EditableLineNode(int id, double x, double y, EditableLine* line) : id(id), xpos(x), ypos(y), line(line)
{
	dr_node.setRadius(node_radius);
	dr_node.setFillColor(sf::Color::Transparent);
	dr_node.setOutlineColor(sf::Color::White);
	dr_node.setOutlineThickness(1.0f);
	dr_node.setOrigin(sf::Vector2f(node_radius, node_radius));
	dr_node.setPosition(sf::Vector2f(xpos, ypos));
}

bool EditableLineNode::pointIntersection(int x, int y) const
{
	if (distanceBetweenAB(x, y, xpos, ypos) <= node_radius) {
		return true;
	}
	return false;
}

void EditableLineNode::translate(double dx, double dy)
{
	xpos += dx;
	ypos += dy;
	adjustDrawables();
}

void EditableLineNode::setPosition(double x, double y)
{
	xpos = x;
	ypos = y;
	adjustDrawables();
}

void EditableLineNode::setLength(double length)
{

}

void EditableLineNode::setAngle(double angle)
{
}

void EditableLineNode::setRecursive(bool b)
{

}

sf::CircleShape& EditableLineNode::getDrNode()
{
	return dr_node;
}

double EditableLineNode::getX() const
{
	return xpos;
}
double EditableLineNode::getY() const
{
	return ypos;
}
int EditableLineNode::getID() const
{
	return id;
}

void EditableLineNode::adjustDrawables()
{
	dr_node.setPosition(sf::Vector2f(xpos, ypos));
	line->adjustDrawables();
}
