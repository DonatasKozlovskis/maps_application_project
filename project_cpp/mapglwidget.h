#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glu.h>
#include <GL/gl.h>
#include <QGLWidget>
#include <map.h>

#include "dijkstra.h"


class MapGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MapGLWidget(QWidget *parent = 0);
    ~MapGLWidget();
    int getXPos();
    int getYPos();

    // Testing
    float getMaxLat();
    float getMinLat();
    float getMaxLon();
    float getMinLon();

    int getNormX();
    int getNormY();

    float getScale();

    QPointF getMovPos();
    QPointF getCamPos();

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
    void drawPoints();                      // 24.12 ------------------------------
    void drawPath();
    void loadTextures();

    void detectPoint(float, float);         // 24.12 -------------------------------

    QPointF movPos;             // Current movement
    QPointF camPos;             // Current camera position with movement adjustments
    QPoint lastPos;             // Last position on the screen
    float scale;                // Current map scaling

    float adjustmentX = 0.5f;
    float adjustmentY = 1.65f;

    float mapGeoCoordinates[4]; // Max and min geographic coordinates of the points in the map
    int mapNormalization[2];    // Normalization values for the map
    GLuint textureID;

    Map *mymap;                 // Map instance

    Dijkstra *mydijkstra;
    vector < vector<Node*> > paths;


    vector <Node*> points;      // Points for painting (testing) --------------------------
};

#endif // GLWIDGET_H
