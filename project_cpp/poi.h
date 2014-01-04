#ifndef POI_H
#define POI_H
#include <QtSql>
#include <QVariant>
#include <QFileDialog>

#include "node.h"

class POI: public Node
{
public:
    POI();
    POI(unsigned int, float, float, QString, QString, QString, QString);
    ~POI();

    QString getName();
    QString getImgPath();
    QString getType();
    QString getAddress();

    void setName(QString);
    void setImgPath(QString);
    void setType(QString);
    void setAdress(QString);

    vector<Node*> radiousPoi(float &, int &);

private:
    vector<string> listPoi;

    unsigned int id;
    QString name;
    QString imgPath;
    QString type;
    QString address;
    float latitude, longitude;
};

#endif // POI_H
