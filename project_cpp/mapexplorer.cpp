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

    float mpoglx = (w - widgetWidth/2) - ui->mapGLWidget->getMovPos().x();      // mpogl - Mouse Pressed OpenGL
    float mpogly = -(h - widgetHeight/2) - ui->mapGLWidget->getMovPos().y();

    // Solving ax + b = y for latitude and longitude
    float aLat = (float)ui->mapGLWidget->getNormX() /
            (ui->mapGLWidget->getMaxLat() - ui->mapGLWidget->getMinLat());
    float bLat = (float)ui->mapGLWidget->getNormX() / 2 - ui->mapGLWidget->getMaxLat()*aLat;

    float aLon = (float)ui->mapGLWidget->getNormY() /
            (ui->mapGLWidget->getMaxLon() - ui->mapGLWidget->getMinLon());
    float bLon = (float)ui->mapGLWidget->getNormY() / 2 - ui->mapGLWidget->getMaxLon()*aLon;

    float mpgeolat = (mpogly*1.65/ui->mapGLWidget->getScale() - bLat)/aLat;
    float mpgeolon = (mpoglx*0.5/ui->mapGLWidget->getScale() - bLon)/aLon;

    QString output("Map moved: \n" + QString::number(ui->mapGLWidget->getMovPos().x()) + " "
                   + QString::number(ui->mapGLWidget->getMovPos().y())

                   + "\n\nWidget Width: \nX: " + QString::number(widgetWidth)
                   + " Y: " + QString::number(widgetHeight)

                   + "\n\nMouse pressed widget 1: \nX: " + QString::number(w)
                   + " Y: "   + QString::number(h)

                   + "\n\nMouse pressed widget 2: \nX: " + QString::number(w - widgetWidth/2)
                   + " Y: "   + QString::number(-h + widgetHeight/2)

                   + "\n\nMouse pressed OpenGL: \nX: " + QString::number(mpoglx)
                   + " Y: "   + QString::number(mpogly)

                   + "\n\nMouse pressed Geo: Lat:" + QString::number(mpgeolat)
                   + " Lon: "   + QString::number(mpgeolon)

                   + "\n\n Scale: " + QString::number(ui->mapGLWidget->getScale())
                   );

    ui->debugLabel->setText(output);
}

// Database stuff
void MapExplorer::connectDatabase(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("bigschneider");
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
