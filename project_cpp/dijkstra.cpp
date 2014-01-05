#include <QTime>
#include "dijkstra.h"

/*
* This algorithm has been inspired in the basis suggested for the following link:
* http://www.programming-techniques.com/2012/01/implementation-of-dijkstras-shortest.html
*/

/**
 * @brief Dijkstra::Dijkstra
 * Default constructor of {@link Dijkstra} class
 * @param m1        Adjacency sparse matrix
 * @param initial   Initial point
 * @param end       Target point
 * @param num       Number of matrix rows and columns
 */
Dijkstra::Dijkstra(Eigen::SparseMatrix<float> m1, int &initial, int &end, int num){
    this->adjMatrix = m1;
    this->source = initial;
    this->dest = end;
    this->numOfVertices = num;

    this->predecessor = new int[this->numOfVertices];
    this->distance = new float[this->numOfVertices];
    this->mark = new bool[this->numOfVertices];
}

/**
 * @brief Dijkstra::Dijkstra
 * Constructor of {@link Dijkstra} class
 * @param m1        Adjacency sparse matrix
 * @param initial   Initial point
 * @param end       Target point
 * @param num       Number of matrix rows and columns
 * @param driving   Specifies the if the route is calculated for driving/walking
 */
Dijkstra::Dijkstra(Eigen::SparseMatrix<float> m1, int &initial, int &end, int num, bool driving){
    this->adjMatrix = m1;
    this->source = initial;
    this->dest = end;
    this->numOfVertices = num;
    this->time = 0;
    this->driving = driving;

    this->predecessor = new int[this->numOfVertices];
    this->distance = new float[this->numOfVertices];
    this->mark = new bool[this->numOfVertices];
}

/**
 * @brief Dijkstra::~Dijkstra
 * Destructor of {@link Dijkstra} class
 */
Dijkstra::~Dijkstra() {
    delete [] predecessor;
    delete [] distance;
    delete [] mark;
}

/**
 * @brief Dijkstra::initialize
 * Initialize the values of the vectors used for calculating the shortest-path
 */
void Dijkstra::initialize(){
    for(int i=0;i<numOfVertices;i++) {
        mark[i] = false;
        predecessor[i] = -1;
        distance[i] = INFINITY;
    }
    distance[source]= 0;
}

/**
 * @brief Dijkstra::getClosestUnmarkedNode
 * @return the number of closest nodes not visited
 */
int Dijkstra::getClosestUnmarkedNode(){
    int minDistance = INFINITY;
    int closestUnmarkedNode;
    for(int i=0;i<numOfVertices;i++) {
        if((!mark[i]) && ( minDistance >= distance[i])) {
            minDistance = distance[i];
            closestUnmarkedNode = i;
        }
    }
    return closestUnmarkedNode;
}

/**
 * @brief Dijkstra::calculateDistance
 * Computes the shortest path
 */
void Dijkstra::calculateDistance(){
    QTime timer;
    timer.start();
    initialize();
    int closestUnmarkedNode;
    int count = 0;
    while(count < numOfVertices) {
        closestUnmarkedNode = getClosestUnmarkedNode();
        mark[closestUnmarkedNode] = true;
        for(int i=0;i<numOfVertices;i++) {
            if((!mark[i]) && (adjMatrix.coeff(closestUnmarkedNode,i)>0) ) {
                if(distance[i] > distance[closestUnmarkedNode]+adjMatrix.coeff(closestUnmarkedNode,i)) {
                    distance[i] = distance[closestUnmarkedNode]+adjMatrix.coeff(closestUnmarkedNode,i);
                    predecessor[i] = closestUnmarkedNode;
                }
            }
        }
        count++;
    }
    cout << "Dijkstra calculation: "<< timer.elapsed() << endl;
}

/**
 * @brief Dijkstra::printPath
 * Recursive function that reconstructs the shortest backwards node by node
 * @param node
 */
void Dijkstra::printPath(int node){
    if(node == source){
        cout<<node<<"..";
        this->result.push_back(node);
    }else if(predecessor[node] == -1){
        cout<<"No path from "<<source<<" to "<<node<<endl;
    }else {
        printPath(predecessor[node]);
        cout<<node<<"..";
        this->result.push_back(node);
    }
}

/**
 * @brief Dijkstra::output
 * @return a vector that contains all nodes of the shortest-path
 */
vector<int> Dijkstra::output(){
    this->result.push_back(this->source);

    int i = this->dest;
        if(i == source){
            cout<<source<<".."<<source;
        }else{
            printPath(i);
            cout<<"->"<<distance[i]*4<<" meters"<<endl;
        }
    calcTime();
    return this->result;
}

/**
 * @brief Dijkstra::calcTime
 * Receives the distance that has to be covered walking/driving
 * and returns the time in minutes that the user would spend
 */
void Dijkstra::calcTime(){
    if (driving == true){
        float car = 11.12f;          //The average speed by car is setted to 40 km/h or 11.12 m/s
        this->time = ((this->distance[this->dest]/car)*4)/60;
        cout<<"Distance: "<<this->distance[this->dest]*4<<" Time: "<<this->time<<" minutes"<<endl;
    }
    if (driving == false){
        float person = 1.12f;        //The average speed by walking is setted to 4 km/h or 1.12 m/s
        this->time = ((this->distance[this->dest]/person)*4)/60;
        cout<<"Distance: "<<this->distance[this->dest]*4<<" Time: "<<this->time<<" minutes"<<endl;
    }
}

float Dijkstra::getDistance() {
        return distance[this->dest]*4;
}

float Dijkstra::getTime() {
//    if (driving)
//        return ((this->distance[this->dest]/11.12f)*4)/60;
    return ((this->distance[this->dest])*4)/60;
}
