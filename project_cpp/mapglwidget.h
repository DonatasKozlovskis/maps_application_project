#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <windows.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <QGLWidget>

#include "map.h"
#include "dijkstra.h"


class MapGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MapGLWidget(QWidget *parent = 0);
    ~MapGLWidget();
    int getXPos();
    int getYPos();

signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int heigth, int width);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

signals:
    void mousePressedGL(int,int);
    void mouseMovedGL(int,int);
    void mouseReleasedGL(int,int);
    void mouseWheeledGL(float);

private:
    void draw();
    void drawMap();
    void drawAxices();
    void drawTextureMap();

    void drawPath();

    void loadTextures();

    QPointF camPos;             // Current camera position with movement adjustments
    QPoint lastPos;             // Last position on the screen
    float scale;                // Current map scaling

    int mapNormalization[2];
    GLuint textureID;

    Map *mymap;
    Map *mymapn;

    Dijkstra *mydijkstra;
    vector <Node*> path;
};

#endif // GLWIDGET_H
