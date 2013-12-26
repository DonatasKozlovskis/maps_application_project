#include "dijkstra.h"

//This algorithm has been inspired in the basis suggested for the following link:
//http://www.programming-techniques.com/2012/01/implementation-of-dijkstras-shortest.html

Dijkstra::Dijkstra(float** adj, int &initial, int &end, int num, bool driving){
    this->adjMatrix = adj;
    this->source = initial;
    this->dest = end;
    this->numOfVertices = num;
    this->time = 0;
    this->driving = driving;

    this->predecessor = new int[this->numOfVertices];
    this->distance = new float[this->numOfVertices];
    this->mark = new bool[this->numOfVertices];
}

// Added Andrey
Dijkstra::~Dijkstra() {
    delete [] predecessor;
    delete [] distance;
    delete [] mark;
}

void Dijkstra::initialize(){
    for(int i=0;i<numOfVertices;i++) {
        mark[i] = false;
        predecessor[i] = -1;
        distance[i] = INFINITY;
    }
    distance[source]= 0;
}

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

void Dijkstra::calculateDistance(){
    initialize();
    int minDistance = INFINITY;
    int closestUnmarkedNode;
    int count = 0;
    while(count < numOfVertices) {
        closestUnmarkedNode = getClosestUnmarkedNode();
        mark[closestUnmarkedNode] = true;
        for(int i=0;i<numOfVertices;i++) {
            if((!mark[i]) && (adjMatrix[closestUnmarkedNode][i]>0) ) {
                if(distance[i] > distance[closestUnmarkedNode]+adjMatrix[closestUnmarkedNode][i]) {
                    distance[i] = distance[closestUnmarkedNode]+adjMatrix[closestUnmarkedNode][i];
                    predecessor[i] = closestUnmarkedNode;
                }
            }
        }
        count++;
    }
}

void Dijkstra::printPath(int node){
    if(node == source){
        cout<<node<<"..";
        //this->result.push_back(node);
    }else if(predecessor[node] == -1){
        cout<<"No path from "<<source<<" to "<<node<<endl;
    }else {
        printPath(predecessor[node]);
        cout<<node<<"..";
        this->result.push_back(node);
    }
}

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

//This function receives the distance that has to be covered walking/driving
//and returns the time in minutes that the user would spend;
void Dijkstra::calcTime(){
    if (driving == true){
        float car = 11.12;          //The average speed by car is setted to 40 km/h or 11.12 m/s
        this->time = ((this->distance[this->dest]/car)*4)/60;
        cout<<"Distance: "<<this->distance[this->dest]*4<<" Time: "<<this->time<<" minutes"<<endl;
    }
    if (driving == false){
        float person = 1.12;        //The average speed by walking is setted to 4 km/h or 1.12 m/s
        this->time = ((this->distance[this->dest]/person)*4)/60;
        cout<<"Distance: "<<this->distance[this->dest]*4<<" Time: "<<this->time<<" minutes"<<endl;
    }
}

