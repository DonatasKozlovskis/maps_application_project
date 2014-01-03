#include <QMouseEvent>
#include <QTime>

#include "mapglwidget.h"

/**
 * @brief MapGLWidget::MapGLWidget
 * Default constructor of the {@link MapGLWidget} class.
 * @param parent
 */
MapGLWidget::MapGLWidget(QWidget *parent) : QGLWidget(parent) {

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    adjustmentX = 1;//0.5;
    adjustmentY = 1;//1.65;

    camPos = QPointF(0.0f,0.0f);            // camera position (uses adjustments)
    movPos = QPointF(0.0f,0.0f);            // movement position (without adjustments)

    // Set scale to 1
    scale = 1;

    // Why the fuck does it works?
    mapNormalization[0] = 2048; //2210;
    mapNormalization[1] = 1024; //670;

    // This is not good as this coordinates are approximate
    // Map's node max/min altitude and max/min longitude
    mapGeoCoordinates[0] = 46.77265f; //46.7696832;   // Min Latitude
    mapGeoCoordinates[1] = 46.8217f; //46.8230965;  // Max Latitude
    mapGeoCoordinates[2] = 4.36545f; //4.3899093;   // Min Longitude
    mapGeoCoordinates[3] = 4.5086f; //4.4844505;   // Max Longitude

    // Create a new map and populate it.
    mymap = new Map;
    QTime timer;
    timer.start();
    mymap->addData();
    cout << "Data add: " << timer.elapsed() << endl;
    timer.start();
    mymap->normalize(mapNormalization[1],mapNormalization[0], mapGeoCoordinates);
    cout << "Data normalization: " << timer.elapsed() << endl;
    bool driving = true;
    mymap->adjMatrix(driving);
    cout << "Adj: "<< timer.elapsed() << endl;

    // POI container contains QMap of all POIs and a QVector of POI types
    container = new POIContainer();
    container->loadData();

    /*int ini = 1000;
    int dest = 1500;

    this->mydijkstra = new Dijkstra(mymap->m1, ini, dest, mymap->getnumberNodes(), true);
    this->mydijkstra->calculateDistance();
    cout<<"calc dist"<<endl;
    vector <int> result = this->mydijkstra->output();

    path = mymap->getPath(result);
    this->directions = new Patch;
    directions->calcPatch(path);
*/

}

/**
 * @brief MapGLWidget::~MapGLWidget
 * Default destructor of the {@link MapGLWidget} class.
 */
MapGLWidget::~MapGLWidget() {
    makeCurrent();
    glDeleteTextures(1,&textureID);
    //cout << "Map Texture Deleted" << endl;
}

/**
 * @brief MapGLWidget::initializeGL
 * Overloaded function of the {@link QGLWidget} class.
 * Initializes OpenGL parameters.
 */
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
    resizeGL(1000,500);                                      // ? checking
}

/**
 * @brief MapGLWidget::paintGL
 * Overloaded function of the {@link QGLWidget}
 */
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

    //drawAxices();                                           // Draw axices for testing purposes
    //drawMap();                                              // Draw the roads
    drawPath();
    drawPoints();
    drawSpecificPOIs();
}

// Size change routine OpenGL
void MapGLWidget::resizeGL(int width, int height) {

    glViewport(0,0,width,height);						// Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    glOrtho(-500, 500, -250, 250 , 0.0, 2.0);

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
    glVertex3f(50.0f,0.0f,0.0f);

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
            for (int j=0; j<road->length(); j++){
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

    glTexCoord2f(0.0f,0.0f);    glVertex3f(-1024.0f, -512.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);   glVertex3f(1024.0f,-512.0f, -1.0f);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(1024.0f,512.0f,-1.0f);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-1024.0f,512.0f,-1.0f);

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

    // Added Andrey 29/12/2013
    // To select the point on the map, the user should click the left mouse button
    if (event->button() == Qt::LeftButton) {

        lastPos = event->pos();
        emit mousePressedGL(event->pos().x(),event->pos().y());

//        float xt = event->pos().x() - width()/2 - movPos.x();
//        float yt = -event->pos().y() + height()/2 - movPos.y();

//        // For now, we do not need more than three points
//        if (!(points.size() >= 3)) {

//            detectPoint(adjustmentX*xt,adjustmentY*yt);
//            // If user picks up more than one point, start to calculate the paths
//            if (points.size() > 1) {

//                int source = points.at(points.size()-2)->getId();
//                int destination = points.at(points.size()-1)->getId();

//                mydijkstra = new Dijkstra(mymap->m1, source,
//                                         destination ,mymap->numberNodes);
//                mydijkstra->calculateDistance();
//                paths.push_back(mymap->getPath(mydijkstra->output()));
//                delete mydijkstra;
//            }
//        }
        int custom = getFirstCustomAllowed();
        if (custom != -1) {
            float xt = event->pos().x() - width()/2 - movPos.x();
            float yt = -event->pos().y() + height()/2 - movPos.y();

            int closest = mymap->findClosest(yt,xt);
            pointsMap.erase(pointsMap.find(custom));
            pointsMap.insert(custom,mymap->nodes.at(closest));

            setCustomAllowed(custom,false);
            recalculatePaths();
            updateGL();
        }

        updateGL();
    } else if (event->button() == Qt::RightButton) {

        // emit something here
    }
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

//    float d = event->delta();
//    cout << d << endl;
//    if (scale <= 2 && scale >= 0.5)
//        if (d > 0)
//            scale += 0.5;
//        else
//            scale -= 0.5;

//    if (scale < 0.5) scale = 0.5;
//    if (scale > 2) scale = 2;

//    adjustmentX = 0.5/scale;
//    adjustmentY = 1.65/scale;

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

//    glPushMatrix();
//    glColor3f(0,0,0);
//    glLineWidth((GLfloat)2.0);
//    glBegin(GL_LINES);

//    for (vector<Node*>::const_iterator i = points.begin(); i != points.end(); i++) {
//        float x = (*i)->getPoint().x;
//        float y = (*i)->getPoint().y;

//        glVertex3f(y-3,x-3,0.0);
//        glVertex3f(y+3,x+3,0.0);

//        glVertex3f(y-3,x+3,0.0);
//        glVertex3f(y+3,x-3,0.0);
//    }

//    glEnd();

//    glPopMatrix();

    glPushMatrix();
    glColor3f(0,0,0);
    glLineWidth((GLfloat)2.0);
    glBegin(GL_LINES);

    for (QMap<int,Node*>::iterator i = pointsMap.begin(); i != pointsMap.end(); i++) {

        float x = (*i)->getPoint().y;
        float y = (*i)->getPoint().x;
        cout << "Draw: " << (*i)->getId() << endl;
        glVertex3f(x-3,y-3,0.0);
        glVertex3f(x+3,y+3,0.0);

        glVertex3f(x-3,y+3,0.0);
        glVertex3f(x+3,y-3,0.0);
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

void MapGLWidget::updateAdjDriving() {
    this->mymap->rmAdjMatrix();
    this->mymap->adjMatrix(true);
}

void MapGLWidget::updateAdjWalking() {
    this->mymap->rmAdjMatrix();
    this->mymap->adjMatrix(false);
}

// Accessor of the POIs container
POIContainer* MapGLWidget::getPois() {
    return container;
}

// x,y,width,height
QPointF MapGLWidget::widgetToGeoCoordinates(QPointF widgetCoordinates) {
    float mpoglx = (widgetCoordinates.x() - width()/2) - movPos.x();      // mpogl - Mouse Pressed OpenGL
    float mpogly = -(widgetCoordinates.y() - height()/2) - movPos.y();

    float aLat = (float)mapNormalization[0] /
            (mapGeoCoordinates[1] - mapGeoCoordinates[0]);
    float bLat = (float)mapNormalization[0] / 2 - mapGeoCoordinates[1]*aLat;

    float aLon = (float)mapNormalization[1] /
            (mapGeoCoordinates[3] - mapGeoCoordinates[2]);
    float bLon = (float)mapNormalization[1] / 2 - mapGeoCoordinates[3]*aLon;

    float mpgeolon = (mpoglx*0.5/scale - bLon)/aLon;
    float mpgeolat = (mpogly*2/scale - bLat)/aLat;

    return QPointF(mpgeolat,mpgeolon);

}

// lat lon
QPointF MapGLWidget::geoToOpenGLCoordinates(QPointF geoCoordinates) {

    float aLat = (float)mapNormalization[0] /
            (mapGeoCoordinates[1] - mapGeoCoordinates[0]);
    float bLat = (float)mapNormalization[0] / 2 - mapGeoCoordinates[1]*aLat;

    float aLon = (float)mapNormalization[1] /
            (mapGeoCoordinates[3] - mapGeoCoordinates[2]);
    float bLon = (float)mapNormalization[1] / 2 - mapGeoCoordinates[3]*aLon;

    float x = geoCoordinates.x()*aLat + bLat;
    float y = geoCoordinates.y()*aLon + bLon;

    return QPointF(y*2,x/2);
}

void MapGLWidget::updateSpecificPOIs(int type) {
    qDeleteAll(specificPOIs);
    specificPOIs = container->getPOITypeFiltered(type);
    updateGL();
}

void MapGLWidget::drawSpecificPOIs() {

    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f, 1.0f);
    glLineWidth(3);
    for (QMap <int, POI*>::iterator i = specificPOIs.begin(); i != specificPOIs.end(); i++) {

        glColor3f(0.0f,0.0f, 1.0f);
        QPointF result = geoToOpenGLCoordinates(QPointF((*i)->getPoint().x,(*i)->getPoint().y));

        float x = result.x();
        float y = result.y();

        glVertex3f(x-3,y-3,0.0);
        glVertex3f(x+3,y+3,0.0);

        glVertex3f(x-3,y+3,0.0);
        glVertex3f(x+3,y-3,0.0);

        glColor3f(1.0f,0.0f, 1.0f);

//        //QPointF result2 = mymap->findClosest(QPointF(y,x));
//        int result2int = mymap->findClosest(y,x);

//        cout << (*i)->getName().toStdString()  << " Node index: " << result2int << endl;

//        float x1 = mymap->nodes.at(result2int)->getPoint().x;
//        float y1 = mymap->nodes.at(result2int)->getPoint().y;

//        cout << "Source: " << x << " " << y
//             << " Closest: " << y1 << " " << x1 << endl;

//        glVertex3f(y1-3,x1-3,0.0);
//        glVertex3f(y1+3,x1+3,0.0);

//        glVertex3f(y1-3,x1+3,0.0);
//        glVertex3f(y1+3,x1-3,0.0);


    }
    glEnd();
}

void MapGLWidget::recalculatePaths() {

    paths.clear();
    if (pointsMap.size() > 1) {

        QMap <int, Node*>::iterator i;
        QMap <int, Node*>::iterator end = pointsMap.end() - 1;

        for (i = pointsMap.begin(); i != end; i++) {
            int source = (*i)->getId();
            int destination = (*(i+1))->getId();

            cout << "source: " << source << endl;
            cout << "destination: " << source << endl;

            mydijkstra = new Dijkstra(mymap->m1,source,destination,mymap->numberNodes);
            mydijkstra->calculateDistance();
            paths.push_back(mymap->getPath(mydijkstra->output()));
            delete mydijkstra;
        }
    }
}

void MapGLWidget::drawPaths() {

    glPushMatrix();
    glLineWidth((GLfloat) 3.0);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);

    for (vector < vector <Node*> >::iterator i = paths.begin(); i != paths.end(); i++)
        for (vector <Node*>::iterator j = (*i).begin(); j != (*i).end(); j++)
            glVertex3f((*j)->getPoint().x,(*j)->getPoint().y,0.0f);

    glEnd();
    glPopMatrix();
}

void MapGLWidget::addPOI(int position, POI* poi) {
    QPointF coordinates = geoToOpenGLCoordinates(QPointF(poi->getPoint().x,poi->getPoint().y));

    int closest = mymap->findClosest(coordinates.y(),coordinates.x());

    // Operating the same pointer as in the map,
    // so no need to delete allocated memory
    pointsMap.erase(pointsMap.find(position));
    pointsMap.insert(position,mymap->nodes.at(closest));

    cout << "POI Added: " << poi->getName().toStdString()
         << " Closest: " << closest;

    cout << " x " << coordinates.x() << " y " << coordinates.y()
            << " x1 " << mymap->nodes.at(closest)->getPoint().x
            << " y1 " << mymap->nodes.at(closest)->getPoint().y << endl;

    recalculatePaths();
    updateGL();

}

void MapGLWidget::removePOI(int index) {
    pointsMap.erase(pointsMap.find(index));
    recalculatePaths();
    updateGL();
}

void MapGLWidget::setCustomAllowed(int index, bool state) {
    customAllowed[index] = state;
}

int MapGLWidget::getFirstCustomAllowed() {
    for (int i = 0; i < MAX_POINTS; i++)
        if (customAllowed[i]) return i;

    return -1;
}
