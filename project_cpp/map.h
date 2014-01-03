#ifndef MAP_H
#define MAP_H

#include <map>
#include <QtSql>
#include <Eigen/Sparse>
#include "road.h"

using namespace std;

/**
 * @brief The Map class
 * The class is used to generate the structure map that is composed by roads and nodes,
 * make the normalization of the nodes and display them the screen, capture the closest
 * node of a point pressed and generate the correspondent adjacency matrix
 */
class Map{

public:
    Map();
    Map(const Map&);
    ~Map();
    void addData();
    void normalize(unsigned int, unsigned int, float*);
    void renderMap();
    Road* getRoad(unsigned int);
    vector<Node*> getPath(vector<int>);
    unsigned int findClosest(float,float);
    void adjMatrix(bool);
    void rmAdjMatrix(); 

    //Accessors
    int getRoadsCount();
    int getNodesCount();
    int getnumberNodes();

    vector<Road*> roads;
    Eigen::SparseMatrix<float> m1;
    map<unsigned int,Node*> nodes;

private:
    int roadsCount;
    int nodesCount;
    int numberNodes;
};

#endif // MAP_H
