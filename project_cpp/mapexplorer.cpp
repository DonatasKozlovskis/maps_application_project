#include <QMessageBox>
#include <QFileInfo>

#include "mapexplorer.h"
#include "ui_mapexplorer.h"

// Default constructor
MapExplorer::MapExplorer(QWidget *parent) : QMainWindow(parent), ui(new Ui::MapExplorer) {
    // Connect to the database
    this->connectDatabase();

    ui->setupUi(this);

    // Connect signals and slots
    connect(ui->mapGLWidget, SIGNAL(mousePressedGL(int,int)), this, SLOT(updateDebugInfo(int,int)));
    connect(ui->mapGLWidget, SIGNAL(mouseMovedGL(int,int)),this, SLOT(updateMapPos(int,int)));
    connect(ui->mapGLWidget, SIGNAL(mouseWheeledGL(float)),this, SLOT(updateMapScale(float)));

    mapMoved = QPoint(0.0d,0.0d);
    mapScale = 1.0f;
}

// Default destructor
MapExplorer::~MapExplorer() {
    delete ui;

    // Disconnect from the database
    disconnectDatabase();
}

// Exit button action
void MapExplorer::on_actionExit_triggered()
{
    QApplication::quit();
}

// About button action
void MapExplorer::on_actionAbout_triggered()
{
    QMessageBox::about(this,    tr("About Map Explorer"),
                                tr("<p>This is a Qt <b>GUI</b> test. Will be used in further projects.</p><p>VIBOT Promotion 8</p>"));
}

void MapExplorer::updateDebugInfo(int w, int h) {
    ui->debugLabel->clear();

    float widgetWidth = ui->mapGLWidget->width();
    float widgetHeight = ui->mapGLWidget->height();

    float mapTexWidth = 2048.0;
    int mapTexHeight = 1024.0;

    float mousePressedTexX = -(widgetWidth/2 - w)/mapScale + mapTexWidth/2 - mapMoved.x()/mapScale;
    float mousePressedTexY = -(widgetHeight/2 - h)/mapScale + mapTexHeight/2 - mapMoved.y()/mapScale;

    QString output("Map moved: " + QString::number(mapMoved.x()) + " " + QString::number(mapMoved.y())

                   + "\n\nWidget Width: X: " + QString::number(widgetWidth)
                   + " Y: " + QString::number(widgetHeight)

                   + "\n\nMouse pressed widget: X: " + QString::number(w)
                   + " Y: "   + QString::number(h)

                   + "\n\nMouse pressed widget center: X: " + QString::number(w-widgetWidth/2)
                   + " Y: "   + QString::number(h-widgetHeight/2)

                   + "\n\nMouse pressed tex: X: " + QString::number(mousePressedTexX)
                   + " Y: "   + QString::number(mousePressedTexY)

                   + "\n\nMouse pressed tex center: X: " + QString::number(mousePressedTexX - 1024)
                   + " Y: "   + QString::number(mousePressedTexY - 512)

                   + "\n\nMouse pressed geo: " + "X :" + QString::number(46.79638985f + (mousePressedTexX-mapTexWidth/2)/2210.0*0.0534133)
                   + " Y :" + QString::number(4.4371799f + (mousePressedTexY-mapTexHeight/2)/670.0*0.0945412)

                   + "\n\n Scale: " + QString::number(mapScale)
                   );

    ui->debugLabel->setText(output);
}

void MapExplorer::updateMapPos(int x, int y) {
    mapMoved.setX(mapMoved.x() + x);
    mapMoved.setY(mapMoved.y() + y);
}

void MapExplorer::updateMapScale(float s) {
    mapScale = s;
}

void MapExplorer::connectDatabase(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("schneider");
    db.setUserName("root");
    db.setPassword("root");

    if (!db.open()) QMessageBox::critical(0, QObject::tr("Database Error"), db.lastError().text());
    else cout << "Database connected MapExplorer" << endl;
}

void MapExplorer::disconnectDatabase() {
    if (db.open()) {
        db.close();
        cout << "Database disconnected MapExplorer" << endl;
    }
}
