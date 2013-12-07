#include "map.h"

Map::Map()
{
    QSqlQuery queryRoads("SELECT  RoadID, Count(RoadID) FROM road_node GROUP BY RoadID ORDER BY RoadID asc;");

    //Prepare the structure
    roadsCount=0;
    nodesCount=0;
    numberNodes = 0;

    while(queryRoads.next()){
        Road* newr=new Road();
        int nodes = queryRoads.value(1).toInt();
        for (int i=0; i<nodes; i++){
            Node* newn=new Node();
            newr->addNode(newn);
            nodesCount++;
        }
        myRoads.push_back(newr);
        roadsCount++;
    }
    cout<<"Roads created: "<<roadsCount<<endl;
    cout<<"Nodes created: "<<nodesCount<<endl;
}

void Map::addData(){

    QSqlQuery queryPoints("SELECT ind, Latitude, Longitude FROM road_node, node WHERE road_node.NodeID=node.ID ORDER BY RoadID asc, ContourOrder asc;");
    QSqlQuery queryNodes("Select max(ind) from node;");
    QSqlQuery queryRoads("select ID, OneWay from Road");

    float lat=0;
    float lon=0;
    unsigned int ID=0;
    bool way;
    Road* roadit;
    Node* nodeit;
    roadsCount=0;
    nodesCount=0;

    queryNodes.next();
    this->numberNodes = queryNodes.value(0).toInt();
    cout<<numberNodes<<endl;

    //Let's add the data
    for(unsigned int i=0; i<myRoads.size(); i++){
        queryRoads.next();
        roadit = myRoads.at(i);
        way=queryRoads.value(1).toBool();
        roadit->setWay(way);
        for (unsigned int j=0; j<roadit->length(); j++){
            queryPoints.next();
            ID=queryPoints.value(0).toInt()-1;        //Add -1 to the index
            lat=queryPoints.value(1).toFloat();
            lon=queryPoints.value(2).toFloat();

            nodeit = roadit->getNode(j);
            nodeit->setId(ID);
            nodeit->setValue(lat,lon);
            nodesCount++;
        }
        roadsCount++;
    }
    cout<<"Roads updated: "<<roadsCount<<endl;
    cout<<"Nodes updated: "<<nodesCount<<endl;
}

Road* Map::getRoad(unsigned int i){
    return myRoads.at(i);
}

void Map::renderMap(){
    for(unsigned int i=0; i<this->myRoads.size(); i++){
        float x,y;
        glBegin(GL_LINE_STRIP);
        for (unsigned int j=0; j<this->getRoad(i)->length(); j++){
                x = this->getRoad(i)->getNode(j)->getValue().x;
                y = this->getRoad(i)->getNode(j)->getValue().y;
                glVertex2f(x,y);
            }
        glEnd();
    }
}

void Map::normalize(){

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
    aLat = 600/dLat;
    aLon = 600/dLon;
    bLat = 300 - (aLat * latMax);
    bLon = 300 - (aLon * lonMax);

    for(unsigned int i=0; i<this->myRoads.size(); i++){
        for (unsigned int j=0; j<this->getRoad(i)->length(); j++){
            x = this->getRoad(i)->getNode(j)->getValue().x;
            y = this->getRoad(i)->getNode(j)->getValue().y;

            xt = x*aLat + bLat;
            yt = (y*aLon + bLon);

            this->getRoad(i)->getNode(j)->setValue(xt, yt);
        }
    }
}

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
    for(unsigned int i=0; i<this->myRoads.size(); i++){
        for (unsigned int j=0; j<this->getRoad(i)->length()-1; j++){
            current = this->getRoad(i)->getNode(j);
            next = this->getRoad(i)->getNode(j+1);
            //Call function distance
            dist = current->distNode(next);
            x = current->getId();                 //The indices of the database starts from 1 !!!!!!! !!!!!!!!
            y = next->getId();
            this->adj[x][y] = dist;
            if (driving == false || this->getRoad(i)->getWay() == false)   this->adj[y][x] = dist;
        }
    }
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
        for(unsigned int i=0; i<this->myRoads.size(); i++){
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



