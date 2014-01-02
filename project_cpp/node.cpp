#include "node.h"

Node::Node(){}

/**
 * @brief Node::Node
 * Default constructor of {@link Node} class
 */
Node::Node(unsigned int nid, float x, float y) {
    point.x = x;
    point.y = y;
    id = nid;
}

// Copy constructor
Node::Node(const Node &node) {
    point.x = node.point.x;
    point.y = node.point.y;

    id = node.getId();
}

// Empty destructor
Node::~Node() {}

/**
 * @brief Node::distNode
 * @param next
 * @return the euclidean distance between two nodes
 */
float Node::distNode(Node* next){
    float x, y, dist;
    x = next->getPoint().x - this->getPoint().x;
    y = next->getPoint().y - this->getPoint().y;

    dist = sqrt(x*x + y*y);
    return dist;
}

// Accessor of the property point
Point Node::getPoint() const{
    return this->point;
}

// Accessor of the propery id
unsigned int Node::getId() const{
    return this->id;
}

// Mutator of the property point
void Node::setPoint(const float &x, const float &y){
    this->point.x=x;
    this->point.y=y;
}

// Mutator of the property id
void Node::setId(unsigned int &s){
    this->id=s;
}
