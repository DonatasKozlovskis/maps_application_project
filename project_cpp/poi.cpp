#include "poi.h"

poi::poi(){}

string poi::getName(){
    return this->name;
}

void poi::setName(string &name){
    this->name = name;
}

//Returns the list of poi stored in the database
vector<string> poi::getListPoi(){
    string name;
    QSqlQuery Poi;
    Poi.prepare("SELECT Name FROM poi");
    Poi.exec();

    while(Poi.next()){
        name = Poi.value(0).toString().toStdString();
        this->listPoi.push_back(name);
    }
    return listPoi;
}

//Obtain the information of a specific poi from the database returning a pointer to this POI
poi* poi::getPoi(string &name){
    poi* request = new poi;
    QVariant _name = QString::fromStdString(name);

    QSqlQuery infoPoi;
    infoPoi.prepare("SELECT * FROM poi WHERE Name=:name");
    infoPoi.bindValue(":name", _name);
    infoPoi.exec();

    float lat, lon;
    string picture;
    unsigned int id, type;

    lat = infoPoi.value(0).toFloat();
    lon = infoPoi.value(1).toFloat();
    type = infoPoi.value(3).toInt();
    picture = infoPoi.value(4).toString().toStdString();
    id = infoPoi.value(6).toInt();

    request->setPoint(lat, lon);
    request->type = type;
    //request->name = name;
    request->pic = picture;
    request->setId(id);

    return request;
}

//Stores an image in specific location
void poi::imagePoi(){

}

//Crates a new poi providing latitude, longitude, type, name and location of the picture
void poi::createPoi(float &lat, float &lon, string &name, int &type, string &pic){

    //Check if a Poi already exists with the same name before creating a new one
    QVariant _name = QString::fromStdString(name);
    QSqlQuery checkPoi;
    checkPoi.prepare("SELECT Name FROM poi WHERE Name=:name");
    checkPoi.bindValue(":name", _name);
    checkPoi.exec();
    if(!checkPoi.first()){
        this->setPoint(lat, lon);
        this->name= name;
        this->type= type;
        this->pic=pic;

        QVariant _name = QString::fromStdString(name);
        QVariant _pic = QString::fromStdString(pic);

        QSqlQuery createPoi;
        createPoi.prepare("INSERT INTO poi (Latitude, Longitude, Name, Type, PicturePath) "
                          "VALUES (:lat, :lon, :name, :type, :pic)");

        createPoi.bindValue(":lat", this->getPoint().x);
        createPoi.bindValue(":lon", this->getPoint().y);
        createPoi.bindValue(":name", _name);
        createPoi.bindValue(":type", this->type);
        createPoi.bindValue(":pic", _pic);
        createPoi.exec();

        cout<<"Point of interest created successfully"<<endl;
    }else{
        cout<<"One Point of interest with the same name already exists"<<endl;
    }
}

//Modifies the name and the type of a poi ussing its ID
void poi::modifyPoi(unsigned int &id, string& name, int &type){
    QVariant _name = QString::fromStdString(name);
    QString Type = "Type";
    QString Name = "Name";

    QSqlQuery modPoi;
    modPoi.prepare(QString("UPDATE poi SET %1 = :type, %2 = :name WHERE ID=:id").arg(Type, Name));
    modPoi.bindValue(":type", type);
    modPoi.bindValue(":name", _name);
    modPoi.bindValue(":id", id);
    modPoi.exec();

    cout<<"Point of interest modifyed successfully"<<endl;
}

//Deletes one poi from the DB using the its ID
void poi::deletePoi(unsigned int &id){
    QSqlQuery deletePoi;
    deletePoi.prepare("DELETE FROM poi WHERE ID= :id");
    deletePoi.bindValue(":id", id);
    deletePoi.exec();

    cout<<"Point of interest deleted successfully"<<endl;
}

//Shows all poi of a specific type around another poi
//setted by the user according to a radius
vector<Node*> poi::radiousPoi(float &radius, int &type){

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
