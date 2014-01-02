#include "road.h"

// Empty constructor
Road::Road() {}

Road::Road(string id) {
    roadID = id;
}

/**
 * @brief Road::Road
 * Constructor of {@link Road} class
 * @param id
 * @param name
 * @param type
 * @param way
 */
Road::Road(string id, string name, string type, bool way) {
    roadID = id;
    roadName = name;
    roadType = type;
    oneWay = way;
}

//Copy constructor
Road::Road(const Road &road) {

    for (vector<Node*>::const_iterator it = road.nodes.begin();
         it != road.nodes.end(); it++)
        nodes.push_back(new Node(**it));

    roadID = road.roadID;
    roadName = road.roadName;
    roadType = road.roadType;

    // passing 'const Road' as 'this' argument of 'bool Road::isOneWay()' discards qualifiers [-fpermissive]
    this->oneWay = road.oneWay;
}

/**
 * @brief Road::~Road
 * Destructor of {@link Road} class
 * Cleans everynode in the road
 */
Road::~Road() {
    for (vector<Node*>::const_iterator it = nodes.begin();
         it != nodes.end(); it++)
        delete *it;
}

/**
 * @brief Road::render
 * Renders all nodes of a road
 */
void Road::render(){
    float x,y;
    glBegin(GL_LINE_STRIP);
        for (unsigned int i=0; i<this->nodes.size(); i++){
            x = this->getNode(i)->getPoint().x;
            y = this->getNode(i)->getPoint().y;
            glVertex2f(x,y);
        }
    glEnd();

}

// Node addition
void Road::addNode(Node* n){
    nodes.push_back(n);
}

// Accessor of the single node
Node* Road::getNode(const unsigned int &i){
    return nodes.at(i);
}

// Accessor of the nodes' length
int Road::length(){
    return this->nodes.size();
}

// Accessor of the property road
string Road::getRoadID() const {
    return roadID;
}

// Mutator of the property  roadID
void Road::setRoadID(const string &newID) {
    roadID = newID;
}

// Accessor of the property roadName
string Road::getRoadName() const {
    return roadName;
}

// Mutator of the property roadName
void Road::setRoadName(const string &newName) {
    roadName = newName;
}

// Accessor of the property roadType
string Road::getRoadType() const {
    return roadType;
}

// Mutator of the propery roadType
void Road::setRoadType(const string & newType) {
    roadType = newType;
}

// Accessor of the property oneWay
bool Road::isOneWay() {
    return oneWay;
}

// Mutator of the property oneWay
void Road::setOneWay(bool way) {
    oneWay = way;
}
