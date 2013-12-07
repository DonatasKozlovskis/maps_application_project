#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
{
    this->mymap = new Map;
    mymap->addData();
    mymap->normalize();
    bool driving = true;
    mymap->adjMatrix(driving);

    int ini = 150;
    int dest =1500;
    this->mydijkstra = new Dijkstra(mymap->adj, ini, dest, mymap->numberNodes);
    this->mydijkstra->calculateDistance();
    vector <int> result = this->mydijkstra->output();
    path = mymap->getPath(result);

}

GLWidget::~GLWidget() {}

void GLWidget::initializeGL() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.5f);
    glClearDepth(10.0f);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(15.0f, 1.0f, 0.0f);

    glLineWidth(1.0f);
    glColor3f(0.0f,1.0f,0.0f);

    draw();
    glColor3f(1.0f,0.0f,0.0f);
    drawPath();

    glPopMatrix();
}

void GLWidget::resizeGL(int width, int height) {

    if (height==0)                      // Prevent A Divide By Zero By
    {
        height=1;                       // Making Height Equal One
    }

    glViewport(0,0,width,height);       // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);        // Select The Projection Matrix
    glLoadIdentity();                   // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    glOrtho(-640.0, 640.0, -400.0, 400.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);         // Select The Modelview Matrix
    glLoadIdentity();
}

void GLWidget::draw()
{
    float x,y;
    for(unsigned int i=0; i<this->mymap->myRoads.size(); i++){

        glBegin(GL_LINE_STRIP);
            Road *road = this->mymap->getRoad(i);
            for (unsigned int j=0; j<road->length(); j++){
                x = road->getNode(j)->getValue().x;
                y = road->getNode(j)->getValue().y;

                glVertex3f(y,x,0.0f);
            }
        glEnd();
    }
}

void GLWidget::drawPath(){
    float x = 0;
    float y = 0;
    glBegin(GL_LINE_STRIP);
        for(unsigned int i=0; i<this->path.size(); i++){
            Node * node = this->path[i];
            x = node->getValue().x;
            y = node->getValue().y;
            //cout<<x<<" "<<y<<endl;
            glVertex3f(y,x,0.0f);
        }
    glEnd();
}

