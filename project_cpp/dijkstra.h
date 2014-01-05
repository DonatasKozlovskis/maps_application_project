#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <QtSql>
#include <vector>
#include <Eigen/Sparse>
#include "node.h"

#define INFINITY 999

using namespace std;

/**
 * @brief The Dijkstra class
 * The class is used to calculate the shortest path between to points in the map
 * Also allows to calculate the time between this two points depending on walking/driving mode
 */
class Dijkstra{

public:
    Dijkstra(Eigen::SparseMatrix<float>, int &, int &, int);
    Dijkstra(Eigen::SparseMatrix<float>, int &, int &, int, bool);
    ~Dijkstra();

    void initialize();
    int getClosestUnmarkedNode();
    void calculateDistance();
    vector<int> output();
    void printPath(int);
    void calcTime();

    float getDistance();
    float getTime();

private:
    Eigen::SparseMatrix<float> adjMatrix;       //Adjacency sparse matrix using Eigen libray
    int * predecessor;
    float * distance;
    bool * mark;
    int source;
    int dest;
    int numOfVertices;
    vector<int> result;
    float time;
    bool driving;
};

#endif // DIJKSTRA_H
