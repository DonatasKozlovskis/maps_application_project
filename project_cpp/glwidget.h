#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "map.h"
#include "dijkstra.h"
#include "patch.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();


protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int,int);

private:
    Map *mymap;
    Dijkstra *mydijkstra;
    vector <Node*> path;
    void draw();
    void drawPath();

};

#endif // GLWIDGET_H
