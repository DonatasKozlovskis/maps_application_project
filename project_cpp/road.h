#ifndef ROAD_H
#define ROAD_H
#include "node.h"

/**
 * @brief The Road class
 * Creates a structure composed by several nodes with specific parameters
 * such as id, name, type or one way. Also it allows to render the road
 */
class Road{

public:
    Road();
    Road(const Road&);
    Road(string);
    Road(string,string,string,bool);
    ~Road();

    void addNode(Node*);
    void render();

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
    int length();

private:
    string roadID;
    string roadName;
    string roadType;
    bool oneWay;
    vector<Node*> nodes;
};

#endif // ROAD_H
