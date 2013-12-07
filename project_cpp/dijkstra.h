#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <QtSql>
#include <vector>

#include "node.h"

#define INFINITY 999
const int INF = -1;

using namespace std;

class Dijkstra
{
public:
    Dijkstra(float**, int &, int &, int);

    void initialize();
    int getClosestUnmarkedNode();
    void calculateDistance();
    vector<int> output();
    void printPath(int);

    Dijkstra(float**, int);
    vector<int> calculate(int &, int &); // el parámetro 'final' es opcional

private:

    float** adjMatrix;
    int * predecessor;
    float * distance;
    bool * mark; //keep track of visited node
    int source;
    int dest;
    int numOfVertices;
    vector<int> result;

};

#endif // DIJKSTRA_H
