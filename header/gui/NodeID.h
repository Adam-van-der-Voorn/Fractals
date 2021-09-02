
#pragma once
#include <string>
#include <functional>

class NodeID {
    public:
    NodeID(int line_id, bool is_front);
    unsigned int lineID() const;
    bool isFront() const;
    std::string toString() const;
    static NodeID nonexistent();
    static NodeID other(NodeID id);


    private:
    unsigned int line_id;
    bool is_front;
};

bool operator== (const NodeID&, const NodeID&);
bool operator!= (const NodeID&, const NodeID&);
std::ostream& operator << (std::ostream& os, const NodeID& nid);

namespace std {
    template <>
    struct hash<NodeID>
    {
        std::size_t operator()(const NodeID& id) const
        {
            std::size_t hash = id.lineID();
            std::size_t is_front = id.isFront();
            // shift line id by 1, put is_front in the empty space
            hash <<= 1;
            hash &= is_front;
            return hash;
        }
    };
}
