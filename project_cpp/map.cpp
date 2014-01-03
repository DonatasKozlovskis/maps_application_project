#include "map.h"
#include <iostream>
#include <string.h>

using namespace std;

/**
 * @brief Map::Map
 * Default constructor of {@link Map} class
 */
Map::Map(){
    roadsCount=0;
    nodesCount=0;
    numberNodes = 0;
}

/**
 * @brief Map::~Map
 * Destructor of {@link Map} class
 * Resizing to (0,0) is the only way to reset the structure
 */
Map::~Map() {
    for (vector<Road*>::const_iterator it = roads.begin(); it != roads.end(); it++)
        delete (*it);
    this->m1.resize(0,0);
}

/**
 * @brief Map::addData
 * Generates all queries necessaries for composing the map structure
 * and fill it at the same time keeping all nodes of all roads in order
 */
void Map::addData(){

    QSqlQuery queryRoads1("SELECT  RoadID, Count(RoadID) FROM road_node GROUP BY RoadID ORDER BY RoadID asc;");
    QSqlQuery queryPoints("SELECT ind, Latitude, Longitude FROM road_node, node WHERE road_node.NodeID=node.ID ORDER BY RoadID asc, ContourOrder asc;");
    QSqlQuery queryNodes("Select max(ind) from node;");
    QSqlQuery queryRoads("select ID, OneWay, RoadType from Road");

    float lat=0;
    float lon=0;
    unsigned int ID=0;
    bool way;
    string type;
    Road* roadit;
    Node* nodeit;
    roadsCount=0;
    nodesCount=0;

    //Prepare the structure
    while(queryRoads1.next()){
        Road* newr=new Road();
        int nodes = queryRoads1.value(1).toInt();
        for (int i=0; i<nodes; i++){
            Node* newn=new Node();
            newr->addNode(newn);
            nodesCount++;
        }
        roads.push_back(newr);
        roadsCount++;
    }

    queryNodes.next();
    this->numberNodes = queryNodes.value(0).toInt();        //number of nodes present in the database
    //cout<"<Total nodes: "<<numberNodes<<endl;
    //cout<<"Nodes created: "<<nodesCount<<endl;
    //cout<<"Roads created: "<<roadsCount<<endl;

    //Let's add the data
    for(unsigned int i=0; i<roads.size(); i++){
        queryRoads.next();
        roadit = roads.at(i);
        way=queryRoads.value(1).toBool();
        type=queryRoads.value(2).toString().toStdString();
        roadit->setOneWay(way);
        roadit->setRoadType(type);
        for (int j=0; j<roadit->length(); j++){
            queryPoints.next();
            ID=queryPoints.value(0).toInt()-1;              //substract 1 to the index because starts from 1 in the DB
            lat=queryPoints.value(1).toFloat();
            lon=queryPoints.value(2).toFloat();

            nodeit = roadit->getNode(j);
            nodeit->setId(ID);
            nodeit->setPoint(lat,lon);
            nodesCount++;

            nodes.insert(pair<int,Node*>(ID,nodeit));
        }
        roadsCount++;
    }
    //cout<<"Nodes updated: "<<nodesCount<<endl;
    //cout<<"Roads updated: "<<roadsCount<<endl;
}

/**
 * @brief Map::getRoad
 * @param i is the index of the map
 * @return the road correspondent to a specified position
 */
Road* Map::getRoad(unsigned int i){
    return roads.at(i);
}

/**
 * @brief Map::renderMap
 * Renders all roads of the map
 */
void Map::renderMap(){
    for(unsigned int i=0; i<this->roads.size(); i++){
        float x,y;
        glBegin(GL_LINE_STRIP);
        for (int j=0; j<this->getRoad(i)->length(); j++){
                x = this->getRoad(i)->getNode(j)->getPoint().x;
                y = this->getRoad(i)->getNode(j)->getPoint().y;
                glVertex2f(x,y);
            }
        glEnd();
    }
}

/**
 * @brief Map::normalize
 * Normalize the values of the coordinates in order to displayed correctly in the screen
 * @param height
 * @param width
 * @param geoCoords
 */
void Map::normalize(unsigned int height, unsigned int width, float* geoCoords){

    //QSqlQuery queryMaxMin("select max(Latitude), max(Longitude), min(Latitude), min(Longitude) from node;");

    float aLat,aLon,bLat,bLon;
    aLat = height/(*(geoCoords+1) - *(geoCoords));                      // ratio between width and the difference in latitude
    bLat = height/2 - (aLat * *(geoCoords+1));
    aLon = width/(*(geoCoords+3) - *(geoCoords+2));                     // ration between height and difference in longitude
    bLon = width/2 - (aLon * *(geoCoords+3));

    for(unsigned int i=0; i<this->roads.size(); i++){
        for (int j=0; j<this->getRoad(i)->length(); j++){
            float x = this->getRoad(i)->getNode(j)->getPoint().x;       // Latitude
            float y = this->getRoad(i)->getNode(j)->getPoint().y;       // Longitude
            this->getRoad(i)->getNode(j)->setPoint(x*aLat + bLat, y*aLon + bLon);
        }
    }
}

/**
 * @brief Map::adjMatrix
 * Creates the adjacency matrix of the distances between all nodes of the map
 * @param driving determines if the matrix will be composed in walking/driving mode
 */
void Map::adjMatrix(bool driving){

    Node * current = new Node;
    Node * next = new Node;
    float dist = 0.0;
    int x,y;
    string s = "footway";

    if (driving == true){
        Eigen::SparseMatrix<float> mat(this->numberNodes,this->numberNodes);
        for(unsigned int i=0; i<this->roads.size(); i++){
            //If the road is a footway we can not use it with the car
            if(this->getRoad(i)->getRoadType()!= s ){
                for (int j=0; j<this->getRoad(i)->length()-1; j++){
                    current = this->getRoad(i)->getNode(j);
                    next = this->getRoad(i)->getNode(j+1);
                    //Call function distance
                    dist = current->distNode(next);
                    x = current->getId();
                    y = next->getId();
                    mat.coeffRef(x,y) = dist;
                    //Include the same value in the oposite direction if the road is both ways
                    if (this->getRoad(i)->isOneWay() == false)  mat.coeffRef(y,x) = dist;
                }
            }
        }
        this->m1 = mat;
        cout<<"Driving adj matrix loaded"<<endl;
    }
    if (driving == false){
        Eigen::SparseMatrix<float> mat(this->numberNodes,this->numberNodes);
        for(unsigned int i=0; i<this->roads.size(); i++){
            for (int j=0; j<this->getRoad(i)->length()-1; j++){
                current = this->getRoad(i)->getNode(j);
                next = this->getRoad(i)->getNode(j+1);
                //Call function distance
                dist = current->distNode(next);
                x = current->getId();
                y = next->getId();
                mat.coeffRef(x,y) = dist;
                mat.coeffRef(y,x) = dist;
            }
        }
        this->m1 = mat;
        cout<<"Walking adj matrix loaded"<<endl;
    }
}

/**
 * @brief Map::getPath
 * Makes the conversion of a vector of index to the nodes themselfs
 * @param in is the vector of index
 * @return a vector containing the nodes
 */
vector<Node*> Map::getPath(vector<int> in){
    vector<Node*> out;
    Node * targetNode = new Node();
    int id = 0;
    for(unsigned int k=0; k<in.size(); k++){
        id = in[k];
        targetNode = this->nodes[id];
        out.push_back(targetNode);
    }

    return out;
}

/**
 * @brief Map::findClosest
 * Returns the closest node of the point pressed on the map screen
 * @param x is the first component of the point pressed
 * @param y is the second component of the point pressed
 * @return the id of the closest point
 */
unsigned int Map::findClosest(float x, float y) {
    unsigned int result = -1;
    float distance = 9999;

    for (map<unsigned int,Node*>::iterator i = nodes.begin(); i != nodes.end(); i++) {
        float c = sqrt( pow(x - i->second->getPoint().x,2) + pow(y - i->second->getPoint().y,2) );
        if (c < distance) {
            distance = c;
            result = i->first;
        }

    }
    return result;
}


QPointF Map::findClosest(QPointF point) {

    QPointF result = QPointF(-1,-1);
    float distance = 9999;      // Test for now

    for (map<unsigned int,Node*>::iterator i = nodes.begin(); i != nodes.end(); i++) {

        float c = sqrt( pow(point.x() - i->second->getPoint().x,2) + pow(point.y() - i->second->getPoint().y,2) );
        if (c < distance) {
            result.setX(i->second->getPoint().x);
            result.setY(i->second->getPoint().y);
        }

    }
    //cout << result << endl;

    return result;
}

/**
 * @brief Map::rmAdjMatrix
 * Removes the adjacency matrix making resize being the only way to perform the task
 */

void Map::rmAdjMatrix(){
    this->m1.resize(0,0);
}

//Accessor of the property roadsCount
int Map::getRoadsCount(){
    return this->roadsCount;
}

//Accessor of the property nodesCount
int Map::getNodesCount(){
    return this->nodesCount;
}

//Accessor of the property numberNodes
int Map::getnumberNodes(){
    return this->numberNodes;
}
