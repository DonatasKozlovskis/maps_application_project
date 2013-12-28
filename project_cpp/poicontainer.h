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
    bool modifyPOI(int &, QString &, QString &, QString &, QString &);
    POI* getPOI(int &);

    int size() ;

private:
    QMap < int, POI* > pois;

};

#endif // POICONTAINER_H
