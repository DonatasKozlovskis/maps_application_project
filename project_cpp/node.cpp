#include "node.h"

Node::Node(){

}

Point Node::getValue(){
    return this->point;
}
unsigned int Node::getId(){
    return this->id;
}

void Node::setValue(float &x, float &y){
    this->point.x=x;
    this->point.y=y;
}

void Node::setId(unsigned int &s){
    this->id=s;
}

float Node::distNode(Node* next){
    float x, y, dist;
    x = next->getValue().x - this->getValue().x;
    y = next->getValue().y - this->getValue().y;

    dist = sqrt(x*x + y*y);
    return dist;
}
