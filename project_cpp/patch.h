#ifndef PATCH_H
#define PATCH_H

#include <QtSql>
#include <vector>
#include <cmath>
#include <iostream>
#include "node.h"
#include "map.h"

using namespace std;

class Patch
{
public:
    Patch();
    bool belong(Node*, Node*);
    void calcPatch(vector<Node*>);
    float checkAngle(Node*, Node*, Node*);
};

#endif // PATCH_H
