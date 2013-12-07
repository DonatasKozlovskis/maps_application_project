#include "poi.h"

poi::poi()
{

}

//Deletes one poi from the DB using the its ID
void poi::deletePoi(unsigned int &id){
    QSqlQuery deletePoi;
    deletePoi.prepare("DELETE FROM poi WHERE ID= :id");
    deletePoi.bindValue(":id", id);
    deletePoi.exec();
}

//Crates a new poi providing latitude, longitude, type, name and location of the picture
void poi::createPoi(float &lat, float &lon, string &name, int &type, string &pic){
    this->setValue(lat, lon);
    this->name= name;
    this->type= type;
    this->pic=pic;

    QVariant _name = QString::fromStdString(name);
    QVariant _pic = QString::fromStdString(pic);

    QSqlQuery createPoi;
    createPoi.prepare("INSERT INTO poi (Latitude, Longitude, Name, Type, PicturePath) "
                      "VALUES (:lat, :lon, :name, :type, :pic)");

    createPoi.bindValue(":lat", this->getValue().x);
    createPoi.bindValue(":lon", this->getValue().y);
    createPoi.bindValue(":name", _name);
    createPoi.bindValue(":type", this->type);
    createPoi.bindValue(":pic", _pic);
    createPoi.exec();
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
}

//Stores an image in specific location
void poi::imagePoi(){

}
