#ifndef POICONTAINER_H
#define POICONTAINER_H

#include <QMap>
#include "poi.h"

class POIContainer
{
public:
    POIContainer();
    ~POIContainer();

    bool loadData();
    bool addPOI(float &, float&, QString &, QString &, QString &, QString &);
    bool removePOI(int &);
    bool modifyPOI(int , QString , QString , QString , QString );
    POI* getPOI(int &);

    QVector < QString > getTypeList();
    QMap < int, POI* > getPOITypeFiltered(QString &);

    int size() ;

private:
    QMap < int, POI* > pois;
    QVector < QString > typeList;

    // http://stackoverflow.com/questions/5777409/how-to-get-last-prepared-and-executed-query-using-qsqlquery
    QString getLastExecutedQuery(const QSqlQuery&);

};

#endif // POICONTAINER_H
