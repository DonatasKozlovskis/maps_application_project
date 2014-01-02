#ifndef NODE_H
#define NODE_H
#include <vector>
#include <iostream>
#include <string.h>
#include <QtOpenGL>

using namespace std;
/**
 * @brief The Point struct
 * Structure that contains two floats as x, y position of the node in the axis
 */
struct Point {
    float x;
    float y;
};

/**
* @brief The Node class
* Contains a node structure composed by it ID and the previous struct
* Also allows to calculate the euclidian distance between two nodes
*/
class Node{

public:
    Node();
    Node(const Node&);
    Node(unsigned int,float,float);
    ~Node();

    Point getPoint() const;
    unsigned int getId() const;
    void setPoint(const float &, const float &);
    void setId(unsigned int &);
    float distNode(Node*);

private:
    Point point;
    unsigned int id;
};

#endif // NODE_H
