#include "map.h"

// Default constructor
Map::Map()
{
    roadsCount=0;
    nodesCount=0;
}

Map::~Map() {
    for (vector<Road*>::const_iterator it = roads.begin(); it != roads.end(); it++)
        delete (*it);
}

void Map::addData(){

    // Select all roads and from the database
    QSqlQuery queryRoads("SELECT RoadID FROM road_node GROUP BY RoadID ORDER BY RoadID asc;");
    // queryRoads.exec();
    // Prepare the queries for the separate road and the node
    QSqlQuery queryRoad, queryPoints, queryNode;

    QSqlQuery queryNodesCount("select count(id) from node;");
    queryNodesCount.next();
    numberNodes = queryNodesCount.value(0).toInt();

    queryPoints.prepare("select road_node.NodeID from road_node where road_node.RoadID = :roadID order by road_node.ContourOrder ASC;");
    queryNode.prepare("select node.ind, node.Latitude, node.Longitude from node where node.ID = :nodeID;");
    queryRoad.prepare("select * from road where road.ID = :roadID;");

    // For each road create a new Road object
    while (queryRoads.next()) {

        // Query separate road and create an instance of class Road
        QString roadID =queryRoads.value(0).toString();

        queryRoad.bindValue(":roadID",roadID);
        queryRoad.exec();
        queryRoad.next();

        Road* newRoad = new Road(roadID.toStdString(),queryRoad.value(1).toString().toStdString(),
                                 queryRoad.value(2).toString().toStdString(),queryRoad.value(3).toBool());

        // Prepare the query to search the points for the selected road
        queryPoints.bindValue(":roadID",roadID);
        queryPoints.exec();

        // For each point from the previous query, create a new Point object
        while (queryPoints.next()) {

            unsigned int nodeID = queryPoints.value(0).toInt();
            Node* newNode = new Node();

            // Query for the parameters of the current node
            queryNode.bindValue(":nodeID", nodeID);
            queryNode.exec();
            queryNode.next();

            // Fill the Node and add it to the road
            unsigned int id = queryNode.value(0).toInt();
            newNode->setId(--id);

            float latitude = queryNode.value(1).toFloat();
            float longitude = queryNode.value(2).toFloat();

            newNode->setPoint(latitude,longitude);
            newRoad->addNode(newNode);
            nodesCount++;
        }

        // Add road to the map
        roads.push_back(newRoad);
        roadsCount++;
    }

}

Road* Map::getRoad(unsigned int i){
    return roads.at(i);
}

void Map::renderMap(){
    for(unsigned int i=0; i<this->roads.size(); i++){
        float x,y;
        glBegin(GL_LINE_STRIP);
        for (unsigned int j=0; j<this->getRoad(i)->length(); j++){
                x = this->getRoad(i)->getNode(j)->getPoint().x;
                y = this->getRoad(i)->getNode(j)->getPoint().y;
                glVertex2f(x,y);
            }
        glEnd();
    }
}

void Map::normalize(unsigned int width, unsigned int height){

    QSqlQuery queryMaxMin("select max(Latitude), max(Longitude), min(Latitude), min(Longitude) from node;");

    float x,y,xt,yt,latMin,latMax,lonMax,lonMin;
    queryMaxMin.next();
    latMin = queryMaxMin.value(2).toFloat();
    latMax = queryMaxMin.value(0).toFloat();
    lonMax = queryMaxMin.value(1).toFloat();
    lonMin = queryMaxMin.value(3).toFloat();

    float dLat,dLon,aLat,aLon,bLat,bLon;
    dLat = latMax - latMin;
    dLon = lonMax - lonMin;
    aLat = width/dLat;
    aLon = height/dLon;
    bLat = width/2 - (aLat * latMax);
    bLon = height/2 - (aLon * lonMax);

    for(unsigned int i=0; i<this->roads.size(); i++){
        for (unsigned int j=0; j<this->getRoad(i)->length(); j++){
            x = this->getRoad(i)->getNode(j)->getPoint().x;
            y = this->getRoad(i)->getNode(j)->getPoint().y;

            xt = x*aLat + bLat;
            yt = (y*aLon + bLon);

            this->getRoad(i)->getNode(j)->setPoint(xt, yt);
        }
    }
}

// TODO: Replace some code to the constructor i order to prevent the memory leak
void Map::adjMatrix(bool driving){
    //Create structure
    this->adj = new float * [this->numberNodes];
    for (unsigned int i = 0; i<this->numberNodes; i++){
        this->adj[i] = new float[this->numberNodes];
    }

    //Initialize matrix to 999 and 0;
    for (unsigned int i = 0; i<this->numberNodes; i++){
        for (unsigned int j = 0; j<this->numberNodes; j++){
            this->adj[i][j]= 999;
            if(i==j)    this->adj[i][i]=0;
        }
    }

    //Compose matrix
    Node * current = new Node;
    Node * next = new Node;
    float dist = 0.0;
    int x,y;
    for(unsigned int i=0; i<this->roads.size(); i++){
        for (unsigned int j=0; j<this->getRoad(i)->length()-1; j++){
            current = this->getRoad(i)->getNode(j);
            next = this->getRoad(i)->getNode(j+1);
            //Call function distance
            dist = current->distNode(next);
            x = current->getId();                 //The indices of the database starts from 1 !!!!!!! !!!!!!!!
            y = next->getId();
            this->adj[x][y] = dist;
            if (driving == false || this->getRoad(i)->isOneWay() == false)   this->adj[y][x] = dist;
        }
    }
    cout << "lol" << endl;
}

vector<Node*> Map::getPath(vector<int> in){

    vector<Node*> out;
    Node * targetNode = new Node();
    int target = 0;
    int id = 0;
    bool exist = false;
    for(unsigned int k=0; k<in.size(); k++){
        id = in[k];
        exist = false;
        for(unsigned int i=0; i<this->roads.size(); i++){
            for (unsigned int j=0; j<this->getRoad(i)->length(); j++){
                target = this->getRoad(i)->getNode(j)->getId();
                if(target == id && exist == false){
                    targetNode = this->getRoad(i)->getNode(j);
                    out.push_back(targetNode);
                    exist = true;
                }
            }
        }
    }
    return out;
}
