#ifndef POICONTAINER_H
#define POICONTAINER_H

#include <QMap>
#include "poi.h"

/**
 * @brief The POIContainer class
 * The class is used to store the POIs from the database in a QMap <int, POI*> variable.
 * The class is also responsible for filtering the POIs by the type specified.
 */
class POIContainer
{
public:
    POIContainer();
    ~POIContainer();

    bool loadData();
    bool addPOI(float, float, QString, int, QString, QString);
    bool removePOI(int);
    bool modifyPOI(int , QString , int , QString , QString );
    POI* getPOI(int);
    QMap < int, QString> getTypeList();
    QMap < int, POI* > getPOITypeFiltered(int);
    QString getPoiType(int);
    int size() ;
    QString getLastExecutedQuery(const QSqlQuery&);


private:
    QMap < int, POI* > pois;            /** An instance of {@link QMap} that contains the POIs*/
    QMap < int, QString> typeList;      /** An instance of {@link QMap} that contains the ids and the names of the corresponding types of the POIs */

};

#endif // POICONTAINER_H
