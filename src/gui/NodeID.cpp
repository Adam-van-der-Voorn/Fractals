#include "NodeID.h"
#include <limits.h>
#include <iostream>

NodeID::NodeID(int line_id, bool is_front) :
line_id(line_id),
is_front(is_front)
{}

bool operator== (const NodeID& a, const NodeID& b) 
{
    return a.isFront() == b.isFront() && a.lineID() == b.lineID();
}

bool operator!= (const NodeID& a, const NodeID& b) 
{
    return !(a==b);
}

std::ostream& operator << (std::ostream& os, const NodeID& nid) 
{
    os << nid.lineID() << "/" << nid.isFront();
    return os;
}

unsigned int NodeID::lineID() const
{
    return line_id;
}

bool NodeID::isFront() const
{
    return is_front;
}

std::string NodeID::toString() const
{
    return std::to_string(line_id) + (is_front ? "F" : "B");
}

NodeID NodeID::nonexistent()
{
    return NodeID(UINT_MAX, true);
}

NodeID NodeID::other(NodeID id) 
{
    return NodeID(id.lineID(), !id.isFront());
}