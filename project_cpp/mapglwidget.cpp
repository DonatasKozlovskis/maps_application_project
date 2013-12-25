#include <QMouseEvent>

#include "mapglwidget.h"

// Default constructor
MapGLWidget::MapGLWidget(QWidget *parent) : QGLWidget(parent){
    camPos = QPointF(0.0,0.0);

    // Set scale to 1
    scale = 1;

    // Manually foun normalization values (we didn't knew the relation between the map in the database
    // and the size of the map texture)
    mapNormalization[0] = 2210;
    mapNormalization[1] = 670;

    // Create a new map and populate it.
    mymap = new Map;
    mymap->addData();
    mymap->normalize(mapNormalization[0],mapNormalization[1]);

    bool driving = true;
    mymap->adjMatrix(driving);

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
    gluLookAt(-camPos.x(),camPos.y(),0,-camPos.x(),camPos.y(),-100,0,1,0);                       // TODO: Test it

    loadTextures();                                         // Load images and bind them to textures
    glEnable(GL_TEXTURE_2D);                                // Enable 2D texures mode
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);            // Activate previously defined array
    drawTextureMap();                                       // Place the texture
    glDisable(GL_TEXTURE_2D);                               // Disable the texture state, otherwise impossible to change the color of the lines

    drawAxices();                                           // Draw axices for testing purposes

    // is it a memory leak???
    //mymapn = new Map(*mymap);
    //mymapn->normalize(mapNormalization[0],mapNormalization[1]);

    //cout << &mymap->roads.at(0) << " " << &mymapn->roads.at(0) << endl;

    //drawMap();                                              // Draw the roads

    drawPath();
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
    glLineWidth((GLfloat) 3.0);

    glPushMatrix();
    glBegin(GL_LINES);

    glColor3f(1.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,200.0f);

    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,200.0f,0.0f);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(200.0f,0.0f,0.0);

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
    glBegin(GL_LINE_STRIP);
        for(unsigned int i=0; i<this->path.size(); i++){
            Node * node = this->path[i];
            x = node->getPoint().x;
            y = node->getPoint().y;
            //cout<<x<<" "<<y<<endl;
            glVertex3f(y,x,0.0f);
        }
    glEnd();
}

// Once mouse is pressed, record the coordinates
void MapGLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
    emit mousePressedGL(event->pos().x(),event->pos().y());
}

// Once mouse is moved, change the camera position and call re-rendering
void MapGLWidget::mouseMoveEvent(QMouseEvent *event) {

    int dx = event->pos().x() - lastPos.x();
    int dy = event->pos().y() - lastPos.y();

    // Adjustment for sync of mouse and map texture movement
    float adjustmentX = 0.5f/scale;
    float adjustmentY = 1.65f/scale;

    camPos.setX(camPos.x()+adjustmentX*(float)dx);
    camPos.setY(camPos.y()+adjustmentY*(float)dy);

    lastPos.setX(lastPos.x()+dx);
    lastPos.setY(lastPos.y()+dy);

    updateGL();

    emit mouseMovedGL(dx,dy);
}

// Once mouse is released, just do some stuff (testing and debugging purposes)
void MapGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseReleasedGL(event->pos().x(),event->pos().y());
}

// Zoom-in/zoom-out
// Scales are: 0.5 1 1.5 2 (Most probably create a texture with different levels of details for each scale value)
void MapGLWidget::wheelEvent(QWheelEvent *event) {

    if (scale <= 2 && scale >= 0.5)
        scale += event->delta()/240.0f;

    if (scale < 0.5) scale = 0.5;
    if (scale > 2) scale = 2;

    updateGL();

    emit mouseWheeledGL(scale);
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
