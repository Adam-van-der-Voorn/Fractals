#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <memory>
#include "AbsLine.h"

struct LFLine;
class EditableLine;

class EditableLineNode
{
public:

	EditableLineNode(int id, double x, double y, EditableLine* line);

	/**
	\param pos the position to check
	\return true if the distance between the given position and start node < node size
	**/
	bool pointIntersection(int x, int y) const;

	/**
	Translates the selected node by the given amount
	\param double dx the horizontal translation
	\param double dy the vertical translation
	**/
	void translate(double dx, double dy);

	/**
	Sets the position of the selected node
	\param double x the new x position
	\param double y the new y position
	**/
	void setPosition(double x, double y);

	/**
	Sets the length of this nodes line.
	The position of this nodes partner node is changed to achieve this.
	\param length the new length of the line
	**/
	void setLength(double length);

	/**
	Sets the angle of this nodes line.
	The position of this nodes partner node is changed to achieve this.
	\param angle the new angle of the line
	**/
	void setAngle(double angle);

	/**
	Sets the recusion status for this nodes line.
	If true the line will recurse.
	**/
	void setRecursive(bool b);

	/**
	\return the x position of this node
	**/
	double getX() const;
	
	/**
	\return the y position of this node
	**/
	double getY() const;

	/**
	\return the id of the node
	**/
	int getID() const;

	static constexpr double NODE_RADIUS = 10;

private:
	int id;
	EditableLine* line;
	double xpos, ypos;
};

