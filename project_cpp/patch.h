#ifndef PATCH_H
#define PATCH_H

#include <QtSql>
#include <cmath>
#include <iostream>
#include <fstream>
#include "node.h"

using namespace std;

class Patch
{
public:
    Patch();
    bool belong(Node*, Node*);
    void calcPatch(vector<Node*>);
    float checkAngle(Node*, Node*, Node*);
    void genTxt(vector<string>);
};

#endif // PATCH_H
