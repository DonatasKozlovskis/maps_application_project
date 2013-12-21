#ifndef NODE_H
#define NODE_H
#include <vector>
#include <string.h>
#include <QtOpenGL>

using namespace std;

struct Point {
    float x;
    float y;
};

class Node
{
public:
    Node();
    Node(const Node&);          // Added Andrey 02.12
    Node(unsigned int,float,float);   // Added Andrey 02.12

    ~Node();

    Point getPoint() const;           // Name changed from getValue() Andrey 02.12
    unsigned int getId() const;

    void setPoint(const float &, const float &);    // Name changed from setValue() Andrey 02.12
    void setId(unsigned int &);

    float distNode(Node*);

private:
    Point point;
    unsigned int id;
};

#endif // NODE_H
