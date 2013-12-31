#include <QtSql>
#include "poicontainer.h"

/**
 * @brief POIContainer::POIContainer
 * Default constructor of the class {@link POIContainer}.
 */
POIContainer::POIContainer() {
    //pois = new QMap<int,POI*>;
}

/**
 * @brief POIContainer::~POIContainer
 * Default destructor of the class {@link POIContainer}.
 */
POIContainer::~POIContainer() {

}

/**
 * @brief POIContainer::loadData
 * Populates the instance of the {@link POIContainer} with the POI information from the database.
 *
 * The function creates two queries, one for POIs and one for POI types. Then, both private variables
 * are populated with the values from the database.
 *
 * @return true if queries were successful, false otherwise.
 */
bool POIContainer::loadData() {

    QSqlQuery queryPOI,queryPOIType;

    if (queryPOI.exec("select * from poi;")) {

        qDebug() << "POI query executed successfully: ";
        qDebug() << getLastExecutedQuery(queryPOI);

        // Populate the POI type vector
        if (queryPOIType.exec("select * from poitype;")) {

            qDebug() << "POI Type query executed successfully: ";
            qDebug() << getLastExecutedQuery(queryPOIType);

            while (queryPOIType.next())
                typeList.insert(queryPOIType.value(0).toInt(),queryPOIType.value(1).toString());

        } else {

            qDebug() << "POI Type failed: ";
            qDebug() << getLastExecutedQuery(queryPOIType);

            return false;
        }

        // Populate the POI QMap
        while (queryPOI.next()) {

            POI* poi = new POI(queryPOI.value(0).toInt(),              // index POI
                                   queryPOI.value(1).toFloat(),            // lat
                                   queryPOI.value(2).toFloat(),            // lon
                                   queryPOI.value(3).toString(),           // name
                                   typeList.value(queryPOI.value(4).toInt()),           // type
                                   queryPOI.value(5).toString(),           // path
                                   queryPOI.value(6).toString());

            pois.insert(queryPOI.value(0).toInt(), poi);

        }

        return true;
    }

    qDebug() << "POI query failed: ";
    qDebug() << getLastExecutedQuery(queryPOI);

    return false;

}

/**
 * @brief POIContainer::size
 *
 * @return the number of POIs in the container
 */
int POIContainer::size() {
    return pois.size();
}

/**
 * @brief POIContainer::getPOI
 *
 * @param index - an index of the requested POI
 *
 * @return the pointer to the POI with the specified index
 */
POI* POIContainer::getPOI(int &index) {
    return pois.value(index);
}

/**
 * @brief POIContainer::addPOI
 * Adds a POI with the parameters specified to the container and to the database.
 *
 * @param lat - latitude of the POI
 * @param lon - longitude of the POI
 * @param nname - name of POI
 * @param ntype - Type of the POI (QString)
 * @param npath - Path to the POI's image (QString)
 * @param naddress - Address of the POI
 *
 * @return true if database insert successful, false otherwise
 */
bool POIContainer::addPOI(float &lat, float &lon, QString &nname, int &ntype, QString &npath, QString &naddress) {

    // I don't know if it is a good way to check for the index that will be assigned to the ne POI once added
    // to the database. It's autoincrement, so it should add maximum index + 1
    QSqlQuery query("select max(id) from schneider.poi;");
    query.next();
    int maxIndex = query.value(0).toInt();

    // Check if the POI with the same name exists
    QSqlQuery checkPoi;
    checkPoi.prepare("SELECT Name FROM poi WHERE Name=:name");
    checkPoi.bindValue(":name", (QVariant)nname);
    checkPoi.exec();

    if (!checkPoi.first()) {
        POI* newPOI = new POI(maxIndex+1,lat,lon,nname,typeList.value(ntype), npath,naddress);

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
            qDebug() << "Point of interest created successfully!";
            qDebug() << getLastExecutedQuery(insertPoi);
            return true;

        } else {

            delete newPOI;
             qDebug() << "Failed to add Point of Interest.";
             qDebug() << getLastExecutedQuery(insertPoi);
            return false;
        }
    }

    qDebug() << "One Point of interest with the same name already exists!" << endl;
    return false;

}

/**
 * @brief POIContainer::removePOI
 * Deletes the POI with the id specified from the container and the database.
 *
 * @param id - an id of the POI to be deleted
 *
 * @return true if deletion was successful, false otherwise
 */
bool POIContainer::removePOI(int &id) {

    QSqlQuery deletePoi;
    deletePoi.prepare("DELETE FROM poi WHERE ID= :id");
    deletePoi.bindValue(":id", id);

    if (deletePoi.exec()) {
        pois.remove(id);
        qDebug() << "Point of interest removed successfully:";
        qDebug() << getLastExecutedQuery(deletePoi);
        return true;
    }

    qDebug() << "Failed to delete Point of interest:";
    qDebug() << getLastExecutedQuery(deletePoi);
    return false;
}

/**
 * @brief POIContainer::modifyPOI
 * Modifies the POI with the id specified.
 *
 * @param id - an id of the POI to be modified
 * @param nname - name of POI
 * @param ntype - Type of the POI (QString)
 * @param npath - Path to the POI's image (QString)
 * @param naddress - Address of the POI
 *
 * @return
 */
bool POIContainer::modifyPOI(int id, QString nname, QString ntype, QString npath, QString naddress) {
    QSqlQuery modPoi;

    QString Type = "Type";
    QString Name = "Name";
    QString Address = "Address";
    QString PicturePath = "PicturePath";

    modPoi.prepare(QString("UPDATE poi SET %1 = :type ,%2 = :name , %3 = :address, %4 = :path WHERE ID = :id")
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

        qDebug() << "Point of interest modified successfully:";
        qDebug() << getLastExecutedQuery(modPoi);
        return true;

    } else {

        qDebug() << "Point of interest modified successfully:";
        qDebug() << getLastExecutedQuery(modPoi);
        return false;
    }
}

/**
 * @brief POIContainer::getTypeList
 *
 * @return the QMap of the POI types (as values) and their indexes (as keys).
 */
QMap <int,QString> POIContainer::getTypeList() {
    return typeList;
}

/**
 * @brief POIContainer::getPOITypeFiltered
 * This function is necessary to filter the POI based o their type.
 *
 * @param type - an index of the type
 * @return the QMap
 */
QMap < int , POI* > POIContainer::getPOITypeFiltered(int type) {

    QSqlQuery poiFiltered;
    QMap < int, POI* > result;

    if (type) {
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
                               typeList.value(poiFiltered.value(4).toInt()),           // type
                               poiFiltered.value(5).toString(),           // path
                               poiFiltered.value(6).toString());

            result.insert(poiFiltered.value(0).toInt(), poi);



        }
        qDebug() << "POIs filtered successfully, type = ";

    } else {

        qDebug() << "POIs failed to filter, type = ";
    }


    qDebug() << getLastExecutedQuery(poiFiltered);
    cout << getLastExecutedQuery(poiFiltered).toStdString() << endl;

    return result;
}

/**
 * @brief POIContainer::getLastExecutedQuery
 * This function is necessary to get QString representation of the last
 * execured query for the specified QSqlQuery
 *
 * @param query - a QSqlQuery instance to be validated
 * @return
 */
QString POIContainer::getLastExecutedQuery(const QSqlQuery& query) {

    QString str = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());

    while (it.hasNext()) {
        it.next();
        str.replace(it.key(),it.value().toString());
    }

    return str;
}

QString POIContainer::getPoiType(int index) {
    return typeList.value(index);
}
