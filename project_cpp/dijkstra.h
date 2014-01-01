#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <QtSql>
#include <vector>
#include <Eigen/Sparse>

#include "node.h"

#define INFINITY 999
const int INF = -1;

using namespace std;

class Dijkstra
{
public:

    Dijkstra(Eigen::SparseMatrix<float>/*float***/, int &, int &, int);
    Dijkstra(/*float***/Eigen::SparseMatrix<float>, int &, int &, int, bool);
    ~Dijkstra();


    void initialize();
    int getClosestUnmarkedNode();
    void calculateDistance();
    vector<int> output();
    void printPath(int);

    Dijkstra(float**, int);
    vector<int> calculate(int &, int &); // el parámetro 'final' es opcional
    void calcTime();

private:

    //float** adjMatrix;
    Eigen::SparseMatrix<float> adjMatrix;
    int * predecessor;
    float * distance;
    bool * mark; //keep track of visited node
    int source;
    int dest;
    int numOfVertices;
    vector<int> result;
    float time;
    bool driving;
};

#endif // DIJKSTRA_H
