#include <QtSql>
#include "poicontainer.h"

// Default constructor
POIContainer::POIContainer() {
    //pois = new QMap<int,POI*>;
}

// Default destructor
POIContainer::~POIContainer() {

}

// Populates the QMap with the POIs form the database
bool POIContainer::loadData() {
    QSqlQuery queryPOI,queryPOIType;
    if (queryPOI.exec("select * from poi;")) {

        // Populate the POI QMap
        while (queryPOI.next()) {

            POI* poi = new POI(queryPOI.value(0).toInt(),              // index POI
                                   queryPOI.value(1).toFloat(),            // lat
                                   queryPOI.value(2).toFloat(),            // lon
                                   queryPOI.value(3).toString(),           // name
                                   queryPOI.value(4).toString(),           // type
                                   queryPOI.value(5).toString(),           // path
                                   queryPOI.value(6).toString());

            pois.insert(queryPOI.value(0).toInt(), poi);

        }

        // Populate the POI type vector
        queryPOIType.exec("select distinct(type) from schneider.poi;");

        while (queryPOIType.next())
            typeList.push_back(queryPOIType.value(0).toString());

        return true;
    }

    return false;

}

// Accessor of the size of the container
int POIContainer::size() {
    return pois.size();
}

// Accessor of the element of the container
POI* POIContainer::getPOI(int &index) {
    return pois.value(index);
}

//Crates a new poi providing latitude, longitude, type, name and location of the picture
bool POIContainer::addPOI(float &lat, float &lon, QString &nname, QString &ntype, QString &npath, QString &naddress) {

    // I don't know if it is a good way to check for the index that will be assigned to the ne POI once added
    // to the database. It's autoincrement, so it should add maximum index + 1
    QSqlQuery query("select max(id) from schneider.poi;");
    query.next();
    int maxIndex = query.value(0).toInt();

    QSqlQuery checkPoi;
    checkPoi.prepare("SELECT Name FROM poi WHERE Name=:name");
    checkPoi.bindValue(":name", (QVariant)nname);
    checkPoi.exec();

    if (!checkPoi.first()) {
        POI* newPOI = new POI(maxIndex+1,lat,lon,nname,ntype, npath,naddress);

        QSqlQuery insertPoi;
        insertPoi.prepare("INSERT INTO poi (Latitude, Longitude, Name, Type, PicturePath) "
                          "VALUES (:lat, :lon, :name, :type, :pic)");

        insertPoi.bindValue(":lat", lat);
        insertPoi.bindValue(":lon", lon);
        insertPoi.bindValue(":name", nname);
        insertPoi.bindValue(":type", ntype);
        insertPoi.bindValue(":pic", npath);

        if (insertPoi.exec()) {
            pois.insert(maxIndex+1,newPOI);
            cout << "Point of interest created successfully!" << endl;
            return true;
        } else {
            delete newPOI;
            cout << "Failed to add Point of Interest." << endl;
            return false;
        }
    }

    cout << "One Point of interest with the same name already exists!" << endl;
    return false;

}

//Deletes one poi from the DB using the its ID
bool POIContainer::removePOI(int &id) {

    QSqlQuery deletePoi;
    deletePoi.prepare("DELETE FROM poi WHERE ID= :id");
    deletePoi.bindValue(":id", id);

    if (deletePoi.exec()) {
        pois.remove(id);
        cout << " Point of interest deleted successfully! " << endl;
        return true;
    }
        cout << "Failed to delete Point of Interest." << endl;
        return false;
}

// Modifies the parameters of the POI based on the POI's id
bool POIContainer::modifyPOI(int id, QString nname, QString ntype, QString npath, QString naddress) {
    QSqlQuery modPoi;

    QString Type = "Type";
    QString Name = "Name";
    QString Address = "Address";
    QString PicturePath = "PicturePath";

    modPoi.prepare(QString("UPDATE poi SET %1 = \":type\", %2 = \":name\", %3 = \"address\", %4 = \"path\" WHERE ID=:id")
                   .arg(Type, Name, Address, PicturePath));
    modPoi.bindValue(":type", ntype);
    modPoi.bindValue(":name", nname);
    modPoi.bindValue(":id", id);
    modPoi.bindValue(":address", naddress);
    modPoi.bindValue(":path", npath);

    if (modPoi.exec()) {

        POI* poi = pois.find(id).value();
        poi->setName(nname);
        poi->setType(ntype);
        poi->setAdress(naddress);
        poi->setImgPath(npath);

        cout << "Point of interest modified successfully" << endl;

        return true;
    }

    QString result = getLastExecutedQuery(modPoi);
    cout << result.toStdString() << endl;

    cout << "Failed to modyfy Point of Interest!" << endl;
    return false;

}

QVector < QString > POIContainer::getTypeList() {
    return typeList;
}

QMap < int , POI* > POIContainer::getPOITypeFiltered(QString &type) {

    QSqlQuery poiFiltered;
    QMap < int, POI* > result;

    if (type.compare("All")) {
        poiFiltered.prepare("select * from poi where type = :type;");
        poiFiltered.bindValue(":type",type);
    } else
        poiFiltered.prepare("select * from poi");

    if (poiFiltered.exec()) {
        while (poiFiltered.next()) {
            POI* poi = new POI(poiFiltered.value(0).toInt(),              // index POI
                               poiFiltered.value(1).toFloat(),            // lat
                               poiFiltered.value(2).toFloat(),            // lon
                               poiFiltered.value(3).toString(),           // name
                               poiFiltered.value(4).toString(),           // type
                               poiFiltered.value(5).toString(),           // path
                               poiFiltered.value(6).toString());

            result.insert(poiFiltered.value(0).toInt(), poi);

        }
    } else {
        cout << "Failed to filter" << endl;
    }

    return result;
}

QString POIContainer::getLastExecutedQuery(const QSqlQuery& query)
{
 QString str = query.lastQuery();
 QMapIterator<QString, QVariant> it(query.boundValues());
 while (it.hasNext())
 {
  it.next();
  str.replace(it.key(),it.value().toString());
 }
 return str;
}
