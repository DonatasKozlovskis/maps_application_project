#ifndef MAP_H
#define MAP_H

#include <QtSql>
#include <map>
#include <Eigen/Sparse>
#include "road.h"

using namespace std;

class Map
{
public:
    Map();
    Map(const Map&);                                // Added Andrey 02.12
    ~Map();
    void addData();
    void renderMap();
    Road* getRoad(unsigned int);
    void normalize(unsigned int, unsigned int, float*);

    unsigned int findClosest(float,float);
    QPointF findClosest(QPointF);

    void adjMatrix(bool);
    void rmAdjMatrix();
    vector<Node*> getPath(vector<int>);
    Eigen::SparseMatrix<float> m1;

    vector<Road*> roads;                            // Changed from myRoads Adrey 02.12
    map<unsigned int,Node*> nodes;                           // Added 24.12
    int roadsCount;
    int nodesCount;
    int numberNodes;

};


#endif // MAP_H
