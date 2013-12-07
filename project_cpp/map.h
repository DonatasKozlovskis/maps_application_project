#ifndef MAP_H
#define MAP_H

#include <QtSql>
#include <iostream>
#include "road.h"

using namespace std;

class Map
{
public:
    Map();
    void addData();
    void renderMap();
    Road* getRoad(unsigned int);
    void normalize();
    void adjMatrix(bool);
    vector<Node*> getPath(vector<int>);
    Node* getCloser(float &, float &);

    float **adj;
    vector<Road*> myRoads;
    int roadsCount;
    int nodesCount;
    int numberNodes;
};


#endif // MAP_H
