#include "poi.h"

// An empty constructor
POI::POI() {}

// Default constructor
POI::POI(unsigned int id, float lat, float lon, QString nname, QString ntype, QString npath, QString naddress)
    : Node(id,lat,lon) {

    name = nname;
    imgPath = npath;
    type = ntype;
    address = naddress;

}

// Default destructor
// ?? Memory leak or not?
// http://stackoverflow.com/questions/3261694/why-base-class-destructor-virtual-is-called-when-a-derived-class-object-is-del
POI::~POI() {}

// Accessor of the property name
QString POI::getName(){
    return name;
}

// Accessor of the property image path
QString POI::getImgPath() {
    return imgPath;
}

// Accessor of the property type
QString POI::getType() {
    return type;
}

// Accessor of the property address
QString POI::getAddress() {
    return address;
}

// Mutator of the property name
void POI::setName(QString &nname){
    name = nname;
}

// Mutator of the property path
void POI::setImgPath(QString path) {
    imgPath = path;
}

// Mutator of the property type
void POI::setType(QString ntype) {
    type = ntype;
}

// Mutator of the property address
void POI::setAdress(QString naddress) {
    address = naddress;
}

//Shows all poi of a specific type around another poi
//setted by the user according to a radius
//Take into account that the point in the middle is the one which we use to call the function
vector<Node*> POI::radiousPoi(float &radius, int &type){

    float lat, lon, dist;
    vector<Node*> Poi;

    QSqlQuery radiusPoi;
    radiusPoi.prepare("SELECT Latitude, Longitude FROM poi WHERE Type=:type");
    radiusPoi.bindValue(":type", type);
    radiusPoi.exec();

    while(radiusPoi.next()){
        lat = radiusPoi.value(0).toFloat();
        lon = radiusPoi.value(1).toFloat();

        Node* iter = new Node();
        iter->setPoint(lat, lon);
        dist = this->distNode(iter);
        dist=dist*4;

        if (dist<=radius)   Poi.push_back(iter);
    }
    return Poi;
}
