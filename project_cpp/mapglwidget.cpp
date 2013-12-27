#include <QMouseEvent>
#include <QTime>

#include "mapglwidget.h"

// Default constructor
MapGLWidget::MapGLWidget(QWidget *parent) : QGLWidget(parent){

    adjustmentX = 0.5;
    adjustmentY = 1.65;

    camPos = QPointF(0.0f,0.0f);            // camera position (uses adjustments)
    movPos = QPointF(0.0f,0.0f);            // movement position (without adjustments)

    // Set scale to 1
    scale = 1;

    // Why the fuck does it works?
    mapNormalization[0] = 2048; //2210;
    mapNormalization[1] = 1024; //670;

    // This is not good as this coordinates are approximate
    // Map's node max/min altitude and max/min longitude
    mapGeoCoordinates[0] = 46.7720; //46.7696832;   // Min Latitude
    mapGeoCoordinates[1] = 46.8210; //46.8230965;  // Max Latitude
    mapGeoCoordinates[2] = 4.3655; //4.3899093;   // Min Longitude
    mapGeoCoordinates[3] = 4.5086; //4.4844505;   // Max Longitude

    // Create a new map and populate it.
    mymap = new Map;
    QTime timer;
    timer.start();
    mymap->addData();
    cout << "Data add: " << timer.elapsed() << endl;
    timer.start();
    mymap->normalize(mapNormalization[0],mapNormalization[1], mapGeoCoordinates);
    cout << "Data normalization: " << timer.elapsed() << endl;
    bool driving = false;
    mymap->adjMatrix(driving);
    cout << "Adj: "<< timer.elapsed() << endl;

    int ini = 1000;
    int dest = 1500;

    this->mydijkstra = new Dijkstra(mymap->adj, ini, dest, mymap->numberNodes, driving);
    this->mydijkstra->calculateDistance();
    vector <int> result = this->mydijkstra->output();

    path = mymap->getPath(result);
    this->directions = new Patch;
    directions->calcPatch(path);

}

// Default destructor
MapGLWidget::~MapGLWidget() {
    makeCurrent();
    glDeleteTextures(1,&textureID);
    //cout << "Map Texture Deleted" << endl;
}

// OpenGL Initalization routine
void MapGLWidget::initializeGL() {

    qglClearColor(Qt::gray);                                // Background color
    glShadeModel(GL_SMOOTH);                                // Shader model

    // Depth
    glClearDepth(10.0f);                                    // Depth buffer setup
    glEnable(GL_DEPTH_TEST);                                // Enables Depth Testing
    glEnable(GL_LEQUAL);                                    // The Type Of Depth Testing To Do

    // Lihting
    glEnable(GL_LIGHTING);                                  // Lighting
    glEnable(GL_LIGHT0);                                    // Lighting source 0
    GLfloat lightPosition[4] = { 0, 0, 1, 0.0 };            // Light Source Position, last parameter 0 - uniform ligting
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);       // Ligtht source settings, directed light in infinity

    // Add colors as it is 3D
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable (GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Perspective
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Really Nice Perspective Calculations

    // ???
    resizeGL(height(),width());                                      // ? checking
}

// OpenGL rendering
void MapGLWidget::paintGL() {

    // Textures
    if (glIsTexture(textureID) == GL_TRUE) {
        //glDeleteTextures(1,textureID);
        //cout << "Map Texture Deleted" << endl;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear Screen And Depth Buffer
    glLoadIdentity();                                       // Reset The Current Modelview Matrix

    // Scale or transalte by z?
    glScalef(scale,scale,1);
    // Note, camera's z is is negative
    gluLookAt(-camPos.x(),camPos.y(),0,-camPos.x(),camPos.y(),-20,0,1,0);                       // TODO: Test it

    loadTextures();                                         // Load images and bind them to textures
    glEnable(GL_TEXTURE_2D);                                // Enable 2D texures mode
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);            // Activate previously defined array
    drawTextureMap();                                       // Place the texture
    glDisable(GL_TEXTURE_2D);                               // Disable the texture state, otherwise impossible to change the color of the lines

    drawAxices();                                           // Draw axices for testing purposes
    //drawMap();                                              // Draw the roads
    drawPath();
    drawPoints();
}

// Size change routine OpenGL
void MapGLWidget::resizeGL(int height, int width) {

    if (height==0)										// Prevent A Divide By Zero By
        height=1;										// Making Height Equal One

    glViewport(0,0,height,width);						// Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    glOrtho(-256, 256, -512, 512 , 0.0, 2.0);
    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();                                   // Clear everything
}

// Just some random painitng
void MapGLWidget::draw() {}

// X - Blue, Y - Green, Z - Red
void MapGLWidget::drawAxices(){

    glPushMatrix();

    glLineWidth((GLfloat) 2.0);

    glBegin(GL_LINES);

    glColor3f(1.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,50.0f);

    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,100.0f,0.0f);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(50.0f,0.0f,0.0);

    glEnd();
    glPopMatrix();

}

// Map rendering function
void MapGLWidget::drawMap() {
    float x,y;
    for(unsigned int i=0; i<this->mymap->roads.size(); i++){
        glPushMatrix();

        glLineWidth((GLfloat)2.0);
        glColor3f(1.0f,0.0f,0.0f);
        glBegin(GL_LINE_STRIP);
            Road *road = this->mymap->getRoad(i);
            for (unsigned int j=0; j<road->length(); j++){
                x = road->getNode(j)->getPoint().x;
                y = road->getNode(j)->getPoint().y;
                glVertex3f(y,x,0.0f);
            }
        glEnd();

        glPopMatrix();
    }
}

// Draw the texture under the map
void MapGLWidget::drawTextureMap() {

    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);

    glTexCoord2f(0.0f,0.0f);    glVertex3f(-512.0f, -1024.0f+30, -1.0f);
    glTexCoord2f(1.0f, 0.0f);   glVertex3f(512.0f,-1024.0f+30, -1.0f);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(512.0f,1024.0f+30,-1.0f);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-512.0f,1024.0f+30,-1.0f);

    glEnd();
}

void MapGLWidget::drawPath(){
    float x = 0;
    float y = 0;
    glPushMatrix();

    glLineWidth((GLfloat) 3.0);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_STRIP);

        for(unsigned int i=0; i < paths.size(); i++){
            for (unsigned int j = 0; j < paths.at(i).size(); j++) {
                Node * node = paths.at(i).at(j);
                x = node->getPoint().x;
                y = node->getPoint().y;
                glVertex3f(y,x,0.0f);
            }
        }

    glEnd();
    glPopMatrix();
}

// Once mouse is pressed, record the coordinates
void MapGLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
    emit mousePressedGL(event->pos().x(),event->pos().y());

    float xt = event->pos().x() - width()/2 - movPos.x();
    float yt = -event->pos().y() + height()/2 - movPos.y();



    // For now, we do not need more than three points
    if (!(points.size() >= 3)) {

        detectPoint(adjustmentX*xt,adjustmentY*yt);
        // If user picks up more than one point, start to calculate the paths
        if (points.size() > 1) {

            int source = points.at(points.size()-2)->getId();
            int destination = points.at(points.size()-1)->getId();

            mydijkstra = new Dijkstra(mymap->adj, source,
                                     destination ,mymap->numberNodes);
            mydijkstra->calculateDistance();
            paths.push_back(mymap->getPath(mydijkstra->output()));
            delete mydijkstra;
        }
    }

    updateGL();

}

// Once mouse is moved, change the camera position and call re-rendering
void MapGLWidget::mouseMoveEvent(QMouseEvent *event) {

    // Get the difference between the current point and the previous position
    int dx = event->pos().x() - lastPos.x();
    int dy = event->pos().y() - lastPos.y();

    // Adjustment for sync of mouse and map texture movement
    camPos.setX(camPos.x()+adjustmentX*(float)dx);
    camPos.setY(camPos.y()+adjustmentY*(float)dy);

    // This thing is similar to the lastPos, but it is not updated
    // on the mousePressEvent
    movPos.setX(movPos.x() + (float)dx);
    movPos.setY(movPos.y() - (float)dy);

    // This thing without adjustmets belongs to glWidget point of view
    lastPos.setX( lastPos.x() + dx );
    lastPos.setY( lastPos.y() + dy );

    // Redraw the scene
    updateGL();

    emit mouseMovedGL(dx,dy);
}

// Once mouse is released, just do some stuff (testing and debugging purposes)
void MapGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseReleasedGL(event->pos().x(),event->pos().y());
}

// Zoom-in/zoom-out
// Scales are: 0.5 1 1.5 2
void MapGLWidget::wheelEvent(QWheelEvent *event) {

    float d = event->delta();
    cout << d << endl;
    if (scale <= 2 && scale >= 0.5)
        if (d > 0)
            scale += 0.5;
        else
            scale -= 0.5;

    if (scale < 0.5) scale = 0.5;
    if (scale > 2) scale = 2;

    adjustmentX = 0.5/scale;
    adjustmentY = 1.65/scale;

    //updateGL();
}

// Load the images and bind them to the texture objects
void MapGLWidget::loadTextures() {

    // Load and bind image to the texture object
    textureID = bindTexture(QPixmap(QString(":/img/map.png")),GL_TEXTURE_2D);
    // Ignore texels exceeding S coordinate (x)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // Ignore texels exceeding T coordinate (y)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Replace the figure's color with the texel color
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //cout << "Map Texture loaded" << endl;

}

// Testing
float MapGLWidget::getMinLat() { return *(mapGeoCoordinates); }
float MapGLWidget::getMaxLat() { return *(mapGeoCoordinates+1); }
float MapGLWidget::getMinLon() { return *(mapGeoCoordinates+2); }
float MapGLWidget::getMaxLon() { return *(mapGeoCoordinates+3); }

int MapGLWidget::getNormX() { return *(mapNormalization); }
int MapGLWidget::getNormY() { return *(mapNormalization+1); }

// When clicked, detect the neares point on map (Euclidiean distance to the nearest node)
void MapGLWidget::detectPoint(float x, float y) {
    int n = mymap->findClosest(y,x);

    cout << "x: " << x << " y: " << y << " num : " << n << endl;
    cout << "nearest: x" << mymap->nodes.find(n)->second->getPoint().y
         << " " << " y: " << mymap->nodes.find(n)->second->getPoint().x << endl;

    cout << "qty pts: " << points.size() << endl;

    points.push_back(mymap->nodes.find(n)->second);
}

void MapGLWidget::drawPoints() {

    glPushMatrix();
    glColor3f(0,0,0);
    glLineWidth((GLfloat)2.0);
    glBegin(GL_LINES);


    for (vector<Node*>::const_iterator i = points.begin(); i != points.end(); i++) {
        float x = (*i)->getPoint().x;
        float y = (*i)->getPoint().y;

        glVertex3f(y-5,x-10,0.0);
        glVertex3f(y+5,x+10,0.0);

        glVertex3f(y-5,x+10,0.0);
        glVertex3f(y+5,x-10,0.0);
    }

    glEnd();

    glPopMatrix();
}

// Accessor of the property scale
float MapGLWidget::getScale() {
    return scale;
}

QPointF MapGLWidget::getMovPos() {
    return movPos;
}

QPointF MapGLWidget::getCamPos() {
    return camPos;
}
