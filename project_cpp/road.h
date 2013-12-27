#ifndef ROAD_H
#define ROAD_H
#include "node.h"

class Road
{
public:
    Road();
    Road(const Road&);              // Added Andrey 02.12
    Road(string);
    Road(string,string,string,bool);
    ~Road();                        // Added Andrey 02.12

    void addNode(Node*);

    // Mutators
    void setRoadID(const string&);
    void setRoadName(const string&);
    void setRoadType(const string&);
    void setOneWay(bool);

    // Accessors
    Node* getNode(const unsigned int&);
    string getRoadID() const;
    string getRoadName() const;
    string getRoadType() const;
    bool isOneWay();

    void render();
    int length();

private:
    string roadID;
    string roadName;
    string roadType;                // Changed from roadtype Andrey 02.12
    bool oneWay;                  // Changed from oneWay Andrey 02.12
    vector<Node*> nodes;            // Changed from roadNodes Andrey 02.12
    int* nodesIndexes;
};

#endif // ROAD_H
