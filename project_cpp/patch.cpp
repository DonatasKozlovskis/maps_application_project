#include "patch.h"


Patch::Patch()
{
}

void Patch::calcPatch(vector<Node*> path){
    bool bel = true;
    cout<<"Go ahead"<<endl;

    //Until the path finishes
    for(unsigned int i=0; i<path.size(); i++){
        //Compare if the couple of points belong to the same road
        bel = this->belong(path[i], path[i+1]);
        //If two points does not belong to the same road means that it is a cross where we have to turn
        if(bel == false){
            float angle= this->checkAngle(path[i-1], path[i], path[i+1]);
            if(angle>180.0) cout<<"turn left"<<endl;
            if(angle<180.0) cout<<"turn right"<<endl;
        }
        cout<<"Go ahead"<<endl;
    }
}

//Returns the angle between two segments defined by the same initial point "Center"
float Patch::checkAngle(Node* M, Node* O, Node* P){
    float OMx = M->getValue().x - O->getValue().x;
    float OMy = M->getValue().y - O->getValue().y;
    float OPx = P->getValue().x - O->getValue().x;
    float OPy = P->getValue().y - O->getValue().y;

    float alpha = (OMx*OPx + OMy*OPy) / (sqrt((OMx*OMx)+(OMy*OMy))*sqrt((OPx*OPx)+(OPy*OPy)));
    float angle = acos(alpha)*(180/3.14159265359);

    return angle;
}

//Compares if two nodes belong to the same road
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
    while(belongId1.next()){
        road1=belongId1.value(0).toString().toStdString();
        while(belongId2.next()){
            road2 = belongId2.value(0).toString().toStdString();
            if(road1 == road2){
                return true;
            }
        }
        //problem
        belongId2.first();
    }
    return false;
}
