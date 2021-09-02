#pragma once
#include <memory>
#include "AbsLine.h"
#include "Vec2.h"

struct LFLine;
class EditableLine;

class EditableLineNode
{
public:

	EditableLineNode(const Vec2& pos, bool is_front);

	/**
	\param point the point to check
	\return true if the distance between the given position and start node < node size
	**/
	bool pointIntersection(Vec2 point) const;

	/**
	Sets the position of the selected node
	\param position the new position
	**/
	void setPosition(Vec2 position);

	/**
	\return the position of this node
	**/
	Vec2 getPosition() const;

	/**
	\return true if this node is at the front of it's line;
	**/
	bool isFront() const;

	static constexpr double NODE_RADIUS = 10;

private:
	Vec2 pos;
	bool is_front;
};

