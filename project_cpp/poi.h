#ifndef POI_H
#define POI_H
#include <QtSql>
#include <QVariant>
#include <QFileDialog>

#include "node.h"

class poi: public Node
{
public:
    poi();
    void deletePoi(unsigned int &);
    void createPoi(float &, float &, string &, int &, string &);
    void modifyPoi(unsigned int &, string &, int &);
    void imagePoi();
    string getName();
    void setName(string &);
    vector<string> getListPoi();
    poi* getPoi(string &);
    vector<Node*> radiousPoi(float &, int &);

private:
    string name;
    int type;
    string pic;
    vector<string> listPoi;
};

#endif // POI_H
