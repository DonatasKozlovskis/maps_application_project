#ifndef NODE_H
#define NODE_H
#include <math.h>
#include <vector>
#include <string.h>
#include <QtOpenGL>

using namespace std;

struct Point{
    float x;
    float y;
};

class Node
{
public:
    Node();
    Point getValue();
    unsigned int getId();
    void setValue(float &, float &);
    void setId(unsigned int &);
    float distNode(Node*);

protected:
    Point point;
    unsigned int id;
};

#endif // NODE_H
