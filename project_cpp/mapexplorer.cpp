#include <QMessageBox>
#include <QFileInfo>

#include "mapexplorer.h"
#include "poicontainer.h"


// Default constructor
MapExplorer::MapExplorer(QWidget *parent) : QMainWindow(parent), ui(new Ui::MapExplorer) {
    // Connect to the database
    this->connectDatabase();

    // Create a child in separate window
    poiwidget = new POIWidget(this);
    poiwidget->setWindowFlags(Qt::Window);

    // Setup UI and ui elements
    ui->setupUi(this);
    container = ui->mapGLWidget->getPois();         // Not good
    poiwidget->setContainer(container);            // Not good
    initializeContextMenu();
    initializeComboBoxes();

    // Connect signals and slots
    connect(ui->mapGLWidget, SIGNAL(mousePressedGL(int,int)), this, SLOT(updateDebugInfo(int,int)));
    connect(ui->driveRadioButton, SIGNAL(clicked()), ui->mapGLWidget, SLOT(updateAdjDriving()));
    connect(ui->walkRadioButton, SIGNAL(clicked()), ui->mapGLWidget, SLOT (updateAdjWalking()));
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

    // Why???
    float mpgeolat = (mpogly*2*ui->mapGLWidget->getScale() - bLat)/aLat;
    float mpgeolon = (mpoglx*0.5/ui->mapGLWidget->getScale() - bLon)/aLon;

    QPointF Test = ui->mapGLWidget->widgetToGeoCoordinates(QPointF(w,h));
    QPointF Test2 = ui->mapGLWidget->geoToOpenGLCoordinates(Test);

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

                   + "\n\nTest : Lat: " + QString::number(Test.x())
                   + " Lon :" + QString::number(Test.y())

                   + "\n\n Test 2: x: " + QString::number(Test2.x())
                   + " y: " + QString::number(Test2.y())

                   );

    ui->debugLabel->setText(output);
}

// Database stuff
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

// Context menu call slot
// Called on right-click
void MapExplorer::on_mapGLWidget_customContextMenuRequested(const QPoint &pos)
{
    QPoint newPos = mapToParent(pos);
    contextMenu->move(newPos.x()+this->pos().x(),newPos.y()+this->pos().y());   // Any other way to translate coordinates
    contextMenu->show();
}

// Context menu initialization
void MapExplorer::initializeContextMenu() {

    // Two actions, that appear once user right-clicks the screen
    newPOIAct = new QAction(tr("Add new POI"),this);
    findNearestAct = new QAction(tr("Find nearest.."),this);

    contextMenu = new QMenu(this);
    contextMenu->addAction(newPOIAct);
    contextMenu->addAction(findNearestAct);

    // Connect the actions to the slots
    connect(newPOIAct, SIGNAL(triggered()), this, SLOT(showPOIWidgetNew()));
}

// Show function for POI widget
// Any way to call it directly from connect()?
void MapExplorer::showPOIWidgetNew() {
    poiwidget->adding();
    poiwidget->show();
}

void MapExplorer::on_driveRadioButton_clicked()
{

}

void MapExplorer::on_walkRadioButton_clicked()
{

}

void MapExplorer::on_managePOIButton_clicked()
{
    poiwidget->browsing();
    poiwidget->show();
}

void MapExplorer::initializeComboBoxes() {

    QVector < QString > poiTypes = container->getTypeList();

    ui->P1TypeComboBox->addItem("Custom");
    ui->P1TypeComboBox->addItem("All");
    ui->P2TypeComboBox->addItem("Custom");
    ui->P2TypeComboBox->addItem("All");
    ui->P3TypeComboBox->addItem("Custom");
    ui->P3TypeComboBox->addItem("All");

    QVector <QString>::Iterator i;

    for (i = poiTypes.begin(); i != poiTypes.end(); i++) {
        ui->P1TypeComboBox->addItem((*i));
        ui->P2TypeComboBox->addItem((*i));
        ui->P3TypeComboBox->addItem((*i));

    }
}
