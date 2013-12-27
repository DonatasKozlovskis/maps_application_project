#include "map.h"

// Default constructor


Map::~Map() {
    for (vector<Road*>::const_iterator it = roads.begin(); it != roads.end(); it++)
        delete (*it);
}

Map::Map()
{
    roadsCount=0;
    nodesCount=0;
    numberNodes = 0;
}

void Map::addData(){

    QSqlQuery queryRoads1("SELECT  RoadID, Count(RoadID) FROM road_node GROUP BY RoadID ORDER BY RoadID asc;");

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

    cout<<"Roads created: "<<roadsCount<<endl;
    cout<<"Nodes created: "<<nodesCount<<endl;

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
    for(unsigned int i=0; i<roads.size(); i++){
        queryRoads.next();
        roadit = roads.at(i);
        way=queryRoads.value(1).toBool();
        roadit->setOneWay(way);
        for (unsigned int j=0; j<roadit->length(); j++){
            queryPoints.next();
            ID=queryPoints.value(0).toInt()-1;        //Add -1 to the index
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
    cout<<"Roads updated: "<<roadsCount<<endl;
    cout<<"Nodes updated: "<<nodesCount<<endl;
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

// 2048 1024
void Map::normalize(unsigned int height, unsigned int width, float* geoCoords){

    //QSqlQuery queryMaxMin("select max(Latitude), max(Longitude), min(Latitude), min(Longitude) from node;");

    float aLat,aLon,bLat,bLon;
    aLat = height/(*(geoCoords+1) - *(geoCoords));           // ratio between width and the difference in latitude
    bLat = height/2 - (aLat * *(geoCoords+1));
    aLon = width/(*(geoCoords+3) - *(geoCoords+2));        // ration between height and difference in longitude
    bLon = width/2 - (aLon * *(geoCoords+3));

    for(unsigned int i=0; i<this->roads.size(); i++){
        for (unsigned int j=0; j<this->getRoad(i)->length(); j++){

            float x = this->getRoad(i)->getNode(j)->getPoint().x;       // Lat
            float y = this->getRoad(i)->getNode(j)->getPoint().y;       // Lon

            this->getRoad(i)->getNode(j)->setPoint(x*aLat + bLat, y*aLon + bLon);
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
    string s = "footway";

    if (driving == true){
        for(unsigned int i=0; i<this->roads.size(); i++){
            if(this->getRoad(i)->getRoadType().compare(s) != 0 ){
                for (unsigned int j=0; j<this->getRoad(i)->length()-1; j++){
                    current = this->getRoad(i)->getNode(j);
                    next = this->getRoad(i)->getNode(j+1);
                    //Call function distance
                    dist = current->distNode(next);
                    x = current->getId();                 //The indices of the database starts from 1 !!!!!!! !!!!!!!!
                    y = next->getId();
                    this->adj[x][y] = dist;
                    if (this->getRoad(i)->isOneWay() == false) this->adj[y][x] = dist;
                }
            }
        }
    }

    if (driving == false){
        for(unsigned int i=0; i<this->roads.size(); i++){
            for (unsigned int j=0; j<this->getRoad(i)->length()-1; j++){
                current = this->getRoad(i)->getNode(j);
                next = this->getRoad(i)->getNode(j+1);
                //Call function distance
                dist = current->distNode(next);
                x = current->getId();                 //The indices of the database starts from 1 !!!!!!! !!!!!!!!
                y = next->getId();
                this->adj[x][y] = dist;
                this->adj[y][x] = dist;
            }
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

unsigned int Map::findClosest(float x, float y) {

    unsigned int result = -1;
    float distance = 9999;      // Test for now
    for (map<unsigned int,Node*>::iterator i = nodes.begin(); i != nodes.end(); i++) {

        float c = sqrt( pow(x - i->second->getPoint().x,2) + pow(y - i->second->getPoint().y,2) );
        if (c < distance) {
            distance = c;
            result = i->first;
        }

    }
    //cout << result << endl;

    return result;
}
