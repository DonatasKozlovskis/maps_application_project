#include "road.h"

Road::Road()
{

}

void Road::addNode(Node* n){
    nodesRoad.push_back(n);
}

Node* Road::getNode(unsigned int i){
    return nodesRoad.at(i);
}

int Road::length(){
    return this->nodesRoad.size();
}

void Road::render(){
    float x,y;
    glBegin(GL_LINE_STRIP);
        for (unsigned int i=0; i<this->nodesRoad.size(); i++){
            x = this->getNode(i)->getValue().x;
            y = this->getNode(i)->getValue().y;
            glVertex2f(x,y);
        }
    glEnd();

}

void Road::setWay(bool direction){
    this->oneWay = direction;
}

bool Road::getWay(){
    return this->oneWay;
}

