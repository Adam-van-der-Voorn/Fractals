#include "FrameState.h"
#include <unordered_map>
#include "EditableLine.h"
#include "EditableLineNode.h"
#include <vector>
#include "NodeID.h"

int getID() {
	static int i = 0;
	return i++;
}

FrameState::FrameState(const AbsLine& base_line_dimensions) :
base_line(EditableLine(getID(), base_line_dimensions))
{ }

void FrameState::addLine(const AbsLine& line_dimensions) 
{
    const int line_id = getID();
    lines.emplace(line_id, EditableLine(line_id, line_dimensions));
    nodes.emplace(NodeID(line_id, true), lines.at(line_id).getNode(true));
    nodes.emplace(NodeID(line_id, false), lines.at(line_id).getNode(false));
}

void FrameState::removeLine(int id) 
{
    nodes.erase({id, true});
    nodes.erase({id, false});
    lines.erase(id);
}

void FrameState::setNodePos(NodeID node_id, Vec2 position) 
{
    EditableLine& line = lines.at(node_id.lineID());
    line.setNodePos(node_id.isFront(), position);
}

void FrameState::setLineRecursiveness(int id, bool isRecursive) 
{
    lines.at(id).setRecursive(isRecursive);
}

void FrameState::setFractal(const std::vector<AbsLine>& fractal) 
{
    this->fractal = fractal;
}

void FrameState::clearSelection() 
{
    selected_nodes.clear();
}

void FrameState::selectNode(NodeID node_id) 
{
    selected_nodes.insert(node_id);
}

const EditableLineNode* FrameState::getNode(NodeID node_id) const
{
    return lines.at(node_id.lineID()).getNode(node_id.isFront());
}

const std::unordered_map<NodeID, const EditableLineNode*>& FrameState::getNodes() const
{
    return nodes;
}

const EditableLine& FrameState::getLine(int line_id) const
{
    return lines.at(line_id);
}

const std::unordered_map<int, EditableLine>& FrameState::getLines() const
{
    return lines;
}

const EditableLine& FrameState::getBaseLine() const
{
    return base_line;
}

const std::unordered_set<NodeID>& FrameState::getSelectedNodes() const
{
    return selected_nodes;
}

const std::vector<AbsLine>& FrameState::getFractal() const
{
    return fractal;
}


