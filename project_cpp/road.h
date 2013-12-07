#ifndef ROAD_H
#define ROAD_H
#include "node.h"

class Road
{
public:
    Road();
    void addNode(Node*);
    Node* getNode(unsigned int);
    void render();
    int length();
    void setWay(bool);
    bool getWay();

private:
    string roadID;
    string roadName;
    string roadtype;
    bool oneWay;
    vector<Node*> nodesRoad;
};

#endif // ROAD_H
