#include "FrameState.h"
#include <unordered_map>
#include "EditableLine.h"
#include "EditableLineNode.h"
#include <unordered_map>
#include <vector>

int getID() {
	static int i = 0;
	return i++;
}

static int delete_me = 0; 

FrameState::FrameState(const AbsLine& base_line_dimensions) :
base_line(EditableLine(getID(), getID(), getID(), base_line_dimensions))
{ }

void FrameState::addLine(const AbsLine& line_dimensions) 
{
    const int line_id = getID(), back_id = getID(), front_id = getID();
    lines.emplace(line_id, EditableLine(line_id, back_id, front_id, line_dimensions));
    EditableLineNode* back_node = lines.at(line_id).backNode();
    EditableLineNode* front_node = lines.at(line_id).frontNode();
    nodes.emplace(back_id, back_node);
    nodes.emplace(front_id, front_node);
    delete_me = nodes.at(back_id)->getPosition().x;
    delete_me = 0;
}

void FrameState::removeLine(int id) 
{
    const EditableLine& line = lines.at(id);
    nodes.erase(line.getBackNode()->getID());
    nodes.erase(line.getFrontNode()->getID());
    lines.erase(id);
}

void FrameState::setNodePos(int id, Vec2 position) 
{
    nodes[id]->setPosition(position);
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

void FrameState::selectNode(int id) 
{
    selected_nodes.insert(id);
}

const std::unordered_map<int, EditableLineNode*>& FrameState::getNodes() const
{
    return nodes;
}

const std::unordered_map<int, EditableLine>& FrameState::getLines() const
{
    return lines;
}

const EditableLine& FrameState::getBaseLine() const
{
    return base_line;
}

const std::unordered_set<int>& FrameState::getSelectedNodes() const
{
    return selected_nodes;
}

const std::vector<AbsLine>& FrameState::getFractal() const
{
    return fractal;
}


