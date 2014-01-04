#include "poi.h"

/**
 * @brief POI::POI
 * An empty constructor.
 */
POI::POI() : Node() {}

/**
 * @brief POI::POI
 * A default constructor.
 *
 * @param id - id of the POI
 * @param lat - latitude
 * @param lon - longitude
 * @param nname - name
 * @param ntype - type (QString, not int)
 * @param npath - image path
 * @param naddress - address
 */
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
/**
 * @brief POI::~POI
 * Default destructor.
 */
POI::~POI() {}

/**
 * @brief POI::getName
 * Accessor of the property name
 * @return the name of the POI.
 */
QString POI::getName(){
    return name;
}

/**
 * @brief POI::getImgPath
 * Accessor of the property imgPath.
 * @return the image path of the POI.
 */
QString POI::getImgPath() {
    return imgPath;
}

/**
 * @brief POI::getType
 * Accessor of the property type
 * @return type of the POI
 */
QString POI::getType() {
    return type;
}

/**
 * @brief POI::getAddress
 * Accessor of the propery address
 * @return
 */
QString POI::getAddress() {
    return address;
}

/**
 * @brief POI::setName
 * Mutator of the property name.
 * @param nname
 */
void POI::setName(QString nname){
    name = nname;
}

/**
 * @brief POI::setImgPath
 * Mutator of the property path.
 * @param path
 */
void POI::setImgPath(QString path) {
    imgPath = path;
}

/**
 * @brief POI::setType
 * Mutator of the property type.
 * @param ntype
 */
void POI::setType(QString ntype) {
    type = ntype;
}

/**
 * @brief POI::setAdress
 * Mutator of the property address.
 * @param naddress
 */
void POI::setAdress(QString naddress) {
    address = naddress;
}

/**
 * @brief POI::radiousPoi
 * Shows all poi of a specific type around another poi set by the user according to a radius.
 * Take into account that the point in the middle is the one which we use to call the function
 * @param radius
 * @param type
 * @return
 */
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
