#include "patch.h"
#include <sstream>

Patch::Patch(){}

/**
 * @brief Patch::calcPatch
 * Gives all the indications for the user in order to follow the shortest-path route
 * @param path is the vector of nodes that composes the shortest path
 */
vector <string> Patch::calcPatch(vector<Node*> path){
    bool bel = true;
    vector<string> route;
    string s;
    string ahead = "Go ahead ";
    string right = " meters and turn right";
    string left = " meters and turn left";
    string end = " meters for arriving!";

    Node* init = path[0];
    Node* dest;

    //Until the path finishes
    for(unsigned int i=1; i<path.size()-1; i++){
        //Compare if the couple of points belong to the same road
        bel = this->belong(path[i-1], path[i]);
        //If two points does not belong to the same road means that it is a cross where we have to turn
        if(bel == false){
            float angle= this->checkAngle(path[i], path[i-1] ,path[i+1]);
            if(angle>=30 && angle<160){

                dest = path[i];
                this->distance = init->distNode(dest);
                init = path[i];
                //Composing the string in order to show and store in 'route'
                ostringstream aux;
                aux << floor(this->distance*4);
                s = ahead+aux.str()+right;
                cout<<s<<endl;
                route.push_back(s);
            }
            if(angle<-10 && angle>-160){

                dest = path[i];
                this->distance = init->distNode(dest);
                init = path[i];
                //Composing the string in order to show and store in 'route'
                ostringstream aux;
                aux << floor(this->distance*4);
                s = ahead+aux.str()+left;
                cout<<s<<endl;
                route.push_back(s);
            }
        }
    }
    this->distance = init->distNode(path.back());
    ostringstream aux;
    aux << floor(this->distance*4);
    s = ahead+aux.str()+end;
    //cout<<s<<endl;
    route.push_back(s);
    //this->genTxt(route);
    return route;
}

/**
 * @brief Patch::checkAngle
 * Returns the angle between two segments defined by the same initial point "Center"
 * Inspirated in http://stackoverflow.com/questions/3486172/angle-between-3-points
 * @param M is the first point in the map
 * @param P is the second point in the map
 * @param O is the center of the angle located in between the previous two
 * @return the angle
 */
float Patch::checkAngle(Node* M, Node* P, Node* O){

    POINTFLOAT ab = { M->getPoint().x - O->getPoint().x, M->getPoint().y - O->getPoint().y };
    POINTFLOAT cb = { M->getPoint().x - P->getPoint().x, M->getPoint().y - P->getPoint().y };

    float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
    float cross = (ab.x * cb.y - ab.y * cb.x); // cross product
    float alpha = atan2(cross, dot);

    return (int) floor(alpha * 180. / 3.14159265359 + 0.5);
}

/**
 * @brief Patch::belong
 * Checks if two nodes belong to the same road
 * @param node1 is the first node to compare
 * @param node2 is the second node to compare
 * @return true or false depending on they are in the same road or not
 */
bool Patch::belong(Node* node1, Node* node2){
    string road1;
    string road2;

    int id1 = node1->getId();
    int id2 = node2->getId();

    QSqlQuery belongId1;
    QSqlQuery belongId2;

    belongId1.prepare("select roadID from road_node, node where road_node.NodeID = node.ID and node.ind= :id");
    belongId1.bindValue(":id", id1);
    belongId1.exec();

    belongId2.prepare("select roadID from road_node, node where road_node.NodeID = node.ID and node.ind= :id");
    belongId2.bindValue(":id", id2);
    belongId2.exec();

    //Take into account that the node can belong to more than one road
    for(int i=0; i<belongId1.size(); i++){
        belongId1.next();
        road1=belongId1.value(0).toString().toStdString();
        for(int j=0; j<belongId2.size()-1; j++){
            belongId2.next();
            road2 = belongId2.value(0).toString().toStdString();
            if(road1 == road2){
                return true;
            }
        }
        belongId2.first();
    }
    return false;
}

/**
 * @brief Patch::genTxt
 * Generates a txt file with name route.txt with all previous instructions
 * The file is deleted overwrited each time the txt is generated in the compile folder
 * @param route is a vector which contains all instructions for the user
 */
void Patch::genTxt(vector<string> route){
    ofstream file;
    file.open("route.txt");
    for(unsigned int i=0; i<route.size(); i++){
        file<<route[i]<<endl;
    }

    file.close();
}
