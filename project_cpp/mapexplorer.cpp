#include <QMessageBox>
#include <QFileInfo>

#include "mapexplorer.h"
#include "poicontainer.h"


/**
 * @brief MapExplorer::MapExplorer
 * Default constructor of the {@link MapExplorer} class.
 *
 * Connects to the database, initializes ComboBoxes, Context Menus, etc.
 * Iitializes the child {@link POIWidget}. Connects signals and slots.
 *
 * @param parent
 */
MapExplorer::MapExplorer(QWidget *parent) : QMainWindow(parent), ui(new Ui::MapExplorer) {
    // Connect to the database
    this->connectDatabase();

    // Create a child in separate window
    poiwidget = new POIWidget(this);
    poiwidget->setWindowFlags(Qt::Window);

    // Setup UI and ui elements
    ui->setupUi(this);

    poiComboBoxes[0] = ui->P1ComboBox;
    poiComboBoxes[1] = ui->P2ComboBox;
    poiComboBoxes[2] = ui->P3ComboBox;

    currentPOIs[0] = NULL;
    currentPOIs[1] = NULL;
    currentPOIs[2] = NULL;

    container = ui->mapGLWidget->getPois();         // Not good
    poiwidget->setContainer(container);            // Not good
    initializeContextMenu();
    initializeComboBoxes();

    // Connect signals and slots
    connect(ui->mapGLWidget, SIGNAL(mousePressedGL(int,int)), this, SLOT(updateDebugInfo(int,int)));
    connect(ui->driveRadioButton, SIGNAL(clicked()), ui->mapGLWidget, SLOT(updateAdjDriving()));
    connect(ui->walkRadioButton, SIGNAL(clicked()), ui->mapGLWidget, SLOT (updateAdjWalking()));

    connect(ui->mapGLWidget, SIGNAL(routeUpdated(QString)), this, SLOT (updateRouteInfo(QString)));

    ui->driveRadioButton->click();

    ui->debugLabel->hide();
    ui->saveRouteButton->setEnabled(false);


}

/**
 * @brief MapExplorer::~MapExplorer
 * Default destructor.
 *
 * Disconnects from the database and deletes the UI.
 */
MapExplorer::~MapExplorer() {

    delete ui;
    disconnectDatabase();
}

/**
 * @brief MapExplorer::on_actionExit_triggered
 * Exit button action, also available via Ctrl + Q shortcut.
 */
void MapExplorer::on_actionExit_triggered()
{
    QApplication::quit();
}

/**
 * @brief MapExplorer::on_actionAbout_triggered.
 * About button action, also available via F1 shortcut.
 * Displays the {@link QMesageBox with the help information.}
 */
void MapExplorer::on_actionAbout_triggered()
{
    QMessageBox::about(this,    tr("About Map Explorer"),
                                tr("<p>This is a Qt <b>GUI</b> test. Will be used in further projects.</p><p>VIBOT Promotion 8</p>"));
}

/**
 * @brief MapExplorer::updateDebugInfo
 * Debug label update function.
 *
 * This function was used in the development, in order to track the conversion
 * between the widget, openGL and geographic coordiates.
 *
 * @param w - width of the {@link MapGLWidget}
 * @param h - height of the {@link MapGLWidget}
 */
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

/**
 * @brief MapExplorer::connectDatabase
 * Database connection function.
 */
void MapExplorer::connectDatabase(){

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("bigschneider");
    db.setUserName("root");
    db.setPassword("root");

    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database conectionError"), db.lastError().text());
        qDebug() << db.lastError().text();
    }

    else qDebug() << "Database connected MapExplorer";
}

/**
 * @brief MapExplorer::disconnectDatabase
 * Database disconnection function
 */
void MapExplorer::disconnectDatabase() {

    if (db.open()) {
        db.close();
        qDebug() << "Database disconnected MapExplorer";
    } else
        qDebug() << "Database already disconnected, no need to close the connection.";
}

/**
 * @brief MapExplorer::on_mapGLWidget_customContextMenuRequested
 * The function displays the context menu once called by right-click from the MapGLWdiget.
 * @param pos - the position of the call (in widget coordinates)
 */
void MapExplorer::on_mapGLWidget_customContextMenuRequested(const QPoint &pos)
{
    cout << pos.x() << " " << pos.y() << endl;
    QPoint newPos = mapToParent(pos);
    contextMenu->move(newPos.x()+this->pos().x(),newPos.y()+this->pos().y());   // Any other way to translate coordinates
    contextMenu->show();

    // Update the possible POI
    poiwidget->setPossiblePOICoords(ui->mapGLWidget->widgetToGeoCoordinates(QPointF(pos.x(),pos.y())));
}

/**
 * @brief MapExplorer::initializeContextMenu
 * Context menu initialization.
 *
 * Creates two actions - one for adding the new POI on right-click selection.
 * Another one for finding newarest POI.
 */
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

/**
 * @brief MapExplorer::showPOIWidgetNew
 * Displays the {@link POIWidget} with the settings to add new POI.
 */
void MapExplorer::showPOIWidgetNew() {
    poiwidget->adding();
    poiwidget->show();
}

/**
 * @brief MapExplorer::on_driveRadioButton_clicked
 * Switch (Driving) between drive and walk road types (adjacency matrices affected).
 */
void MapExplorer::on_driveRadioButton_clicked()
{

}

/**
 * @brief MapExplorer::on_driveRadioButton_clicked
 * Switch (Walking) between drive and walk road types (adjacency matrices affected).
 */
void MapExplorer::on_walkRadioButton_clicked()
{

}

/**
 * @brief MapExplorer::on_managePOIButton_clicked
 * Displays the {@link POIWidget} with the setting set to browsing current POIs.
 */
void MapExplorer::on_managePOIButton_clicked()
{
    poiwidget->setPossiblePOICoords(QPointF(-1,-1));
    poiwidget->browsing();
    poiwidget->show();
}

/**
 * @brief MapExplorer::initializeComboBoxes
 * Initializes the ComboBoxes of the {@link MapExplorer} window.
 *
 * The function populates the POI types' ComboBoxes and then updates the POI ComboBoxes.
 * (In this function it only dispables it because the "Custom" text in the POI type ComboBox
 * corresponds to manual point picking on the map and the POI ComboBox is disabled.)
 */
void MapExplorer::initializeComboBoxes() {

    QMap <int,QString> poiTypes = container->getTypeList();

    ui->P1TypeComboBox->addItem("Custom",QVariant(-1));
    ui->P1TypeComboBox->addItem("All", QVariant(0));
    ui->P2TypeComboBox->addItem("Custom",QVariant(-1));
    ui->P2TypeComboBox->addItem("All", QVariant(0));
    ui->P3TypeComboBox->addItem("Custom",QVariant(-1));
    ui->P3TypeComboBox->addItem("All",QVariant(0));

    ui->poiDisplayComboBox->addItem("None",QVariant(-1));
    ui->poiDisplayComboBox->addItem("All",QVariant(0));

    QMap <int,QString>::Iterator i;

    for (i = poiTypes.begin(); i != poiTypes.end(); i++) {
        ui->P1TypeComboBox->addItem(i.value(),(QVariant)i.key());
        ui->P2TypeComboBox->addItem(i.value(),(QVariant)i.key());
        ui->P3TypeComboBox->addItem(i.value(),(QVariant)i.key());
        ui->poiDisplayComboBox->addItem(i.value(), (QVariant)i.key());
    }

    // Update the POI ComboBoxes
    updateP1ComboBox();
    updateP2ComboBox();
    updateP3ComboBox();

}

/**
 * @brief MapExplorer::updateP1ComboBox
 * Updates the POI #1 ComboBox. Disables if "Custom" type is selected.
 *
 * Deprecated - replaced by the {@link MapExplorer::updateComboBox(int index, int type)} function.
 */
void MapExplorer::updateP1ComboBox() {
    int typeSelected = ui->P1TypeComboBox->itemData(ui->P1TypeComboBox->currentIndex()).toInt();

    qDeleteAll(currentComboBoxPOIs[0]);
    ui->P1ComboBox->clear();

    if (typeSelected != -1) {
        currentComboBoxPOIs[0] = container->getPOITypeFiltered(typeSelected);
        QMap <int, POI*>::iterator i;

        for (i = currentComboBoxPOIs[0].begin(); i != currentComboBoxPOIs[0].end(); i++)
            ui->P1ComboBox->addItem( (*i)->getName(), QVariant((*i)->getId()));

        ui->P1ComboBox->setEnabled(true);

    } else {
        ui->P1ComboBox->setEnabled(false);
    }
}

/**
 * @brief MapExplorer::updateP2ComboBox
 * Updates the POI #2 ComboBox. Disables if "Custom" type is selected.
 *
 * Deprecated - replaced by the {@link MapExplorer::updateComboBox(int index, int type)} function.
 */
void MapExplorer::updateP2ComboBox() {
    int typeSelected = ui->P2TypeComboBox->itemData(ui->P2TypeComboBox->currentIndex()).toInt();

    qDeleteAll(currentComboBoxPOIs[1]);
    ui->P2ComboBox->clear();

    if (typeSelected != -1) {
        currentComboBoxPOIs[1] = container->getPOITypeFiltered(typeSelected);
        QMap <int, POI*>::iterator i;

        for (i = currentComboBoxPOIs[1].begin(); i != currentComboBoxPOIs[1].end(); i++)
            ui->P2ComboBox->addItem( (*i)->getName(), QVariant((*i)->getId()));

        ui->P2ComboBox->setEnabled(true);

    } else {
        ui->P2ComboBox->setEnabled(false);
    }
}

/**
 * @brief MapExplorer::updateP3ComboBox
 * Updates the POI #3 ComboBox. Disables if "Custom" type is selected.
 *
 * Deprecated - replaced by the {@link MapExplorer::updateComboBox(int index, int type)} function.
 */
void MapExplorer::updateP3ComboBox() {
    int typeSelected = ui->P3TypeComboBox->itemData(ui->P3TypeComboBox->currentIndex()).toInt();

    qDeleteAll(currentComboBoxPOIs[2]);
    ui->P3ComboBox->clear();

    if (typeSelected != -1) {
        currentComboBoxPOIs[2] = container->getPOITypeFiltered(typeSelected);
        QMap <int, POI*>::iterator i;

        for (i = currentComboBoxPOIs[2].begin(); i != currentComboBoxPOIs[2].end(); i++)
            ui->P3ComboBox->addItem( (*i)->getName(), QVariant((*i)->getId()));

        ui->P3ComboBox->setEnabled(true);

    } else {
        ui->P3ComboBox->setEnabled(false);
    }
}

/**
 * @brief MapExplorer::on_P1TypeComboBox_currentIndexChanged
 * Updates the POI ComboBox accroding to the type selected.
 * @param index
 */
void MapExplorer::on_P1TypeComboBox_currentIndexChanged(int index)
{
    //updateP1ComboBox();
    int typeSelected = ui->P1TypeComboBox->itemData(index).toInt();
    updateComboBox(0,typeSelected);
    //if ()
}

/**
 * @brief MapExplorer::on_P1TypeComboBox_currentIndexChanged
 * Updates the POI ComboBox accroding to the type selected.
 * @param index
 */
void MapExplorer::on_P2TypeComboBox_currentIndexChanged(int index)
{
    //updateP2ComboBox();
    updateComboBox(1,ui->P2TypeComboBox->itemData(index).toInt());
}

/**
 * @brief MapExplorer::on_P1TypeComboBox_currentIndexChanged
 * Updates the POI ComboBox accroding to the type selected.
 * @param index
 */
void MapExplorer::on_P3TypeComboBox_currentIndexChanged(int index)
{
    //updateP3ComboBox();
    updateComboBox(2,ui->P3TypeComboBox->itemData(index).toInt());

}

/**
 * @brief ::MapExplorer::updateComboBox
 * The function updates the ComboBox stored in the {@link MapExplorer::poiComboBoxes},
 * according to the index specified.
 *
 * @param index - the index of the ComboBox (0,1 or 2)
 * @param type - the POI type selected
 */
void::MapExplorer::updateComboBox(int index, int type) {

    cout << "Before deleteion: " << currentComboBoxPOIs[index].count() << endl;
    qDeleteAll(currentComboBoxPOIs[index]);
    currentComboBoxPOIs[index].clear();
    cout << "After deleteion: " << currentComboBoxPOIs[index].count() << endl;
    poiComboBoxes[index]->setDisabled(true);
    poiComboBoxes[index]->clear();

    if (type != -1) {
        currentComboBoxPOIs[index] = container->getPOITypeFiltered(type);
        QMap <int, POI*>::iterator i;

        for (i = currentComboBoxPOIs[index].begin(); i != currentComboBoxPOIs[index].end(); i++)
            poiComboBoxes[index]->addItem( (*i)->getName(), QVariant((*i)->getId()));

        poiComboBoxes[index]->setEnabled(true);
        ui->mapGLWidget->setCustomAllowed(index,false);

    } else {
        poiComboBoxes[index]->setEnabled(false);
        ui->mapGLWidget->setCustomAllowed(index,true);
    }
}

/**
 * @brief MapExplorer::on_poiDisplayComboBox_currentIndexChanged
 * @param index
 */
void MapExplorer::on_poiDisplayComboBox_currentIndexChanged(int index)
{
    if (ui->poiDisplayComboBox->itemData(index).toInt() != -1) {
        ui->mapGLWidget->updateSpecificPOIs(ui->poiDisplayComboBox->itemData(index).toInt());
    }
}
/**
 * @brief MapExplorer::on_P1ComboBox_currentIndexChanged
 * @param index
 */
void MapExplorer::on_P1ComboBox_currentIndexChanged(int index)
{
    if (ui->P1ComboBox->count() != 0) {
        currentPOIs[0] = container->getPOI(ui->P1ComboBox->itemData(index).toInt());
        ui->mapGLWidget->addPOI(0,currentPOIs[0]);
    } else {
        ui->mapGLWidget->setCustomAllowed(0,true);
        ui->mapGLWidget->removePOI(0);
    }
}

/**
 * @brief MapExplorer::on_P2ComboBox_currentIndexChanged
 * @param index
 */
void MapExplorer::on_P2ComboBox_currentIndexChanged(int index)
{
    if (ui->P2ComboBox->count() != 0) {
        currentPOIs[1] = container->getPOI(ui->P2ComboBox->itemData(index).toInt());
        ui->mapGLWidget->addPOI(1,currentPOIs[1]);
    } else {
        ui->mapGLWidget->setCustomAllowed(1,true);
        ui->mapGLWidget->removePOI(1);
    }
}

/**
 * @brief MapExplorer::on_P3ComboBox_currentIndexChanged
 * @param index
 */
void MapExplorer::on_P3ComboBox_currentIndexChanged(int index)
{
    if (ui->P3ComboBox->count() != 0) {
        currentPOIs[2] = container->getPOI(ui->P3ComboBox->itemData(index).toInt());
        ui->mapGLWidget->addPOI(2,currentPOIs[2]);
    } else {
        ui->mapGLWidget->setCustomAllowed(2,true);
        ui->mapGLWidget->removePOI(2);
    }
}

/**
 * @brief MapExplorer::on_deleteP1Button_clicked
 * Resets the selection of the first POI.
 */
void MapExplorer::on_deleteP1Button_clicked()
{
    ui->P1TypeComboBox->setCurrentIndex(0);
    ui->mapGLWidget->removePOI(0);
    ui->mapGLWidget->setCustomAllowed(0,true);
}

/**
 * @brief MapExplorer::on_deleteP2Button_clicked
 * Resets the selection of the second POI.
 */
void MapExplorer::on_deleteP2Button_clicked()
{
    ui->P2TypeComboBox->setCurrentIndex(0);
    ui->mapGLWidget->removePOI(1);
    ui->mapGLWidget->setCustomAllowed(1,true);
}

/**
 * @brief MapExplorer::on_deleteP3Button_clicked
 * Resets the selection of the third POI.
 */
void MapExplorer::on_deleteP3Button_clicked()
{
    ui->P3TypeComboBox->setCurrentIndex(0);
    ui->mapGLWidget->removePOI(2);
    ui->mapGLWidget->setCustomAllowed(2,true);
}

/**
 * @brief MapExplorer::on_saveRouteButton_clicked
 * Save route info button action. Disabled if info is empty.
 */
void MapExplorer::on_saveRouteButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save file","",tr("Plaint Text (*.txt)"));
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly|QFile::Truncate)) {
        file.write(QByteArray(ui->routeInfoTextBrowser->toPlainText().toStdString().c_str()));
        file.close();
    } else {
        QMessageBox::critical(this,"Error","An error occurder while saving the file.");
    }
}

void MapExplorer::updateRouteInfo(QString info) {

    ui->routeInfoTextBrowser->clear();

    if (!info.isEmpty()) {
    ui->routeInfoTextBrowser->append(info);
    ui->routeInfoTextBrowser->verticalScrollBar()->setValue(ui->routeInfoTextBrowser->verticalScrollBar()->minimum());
    ui->saveRouteButton->setEnabled(true);
    } else {
        ui->saveRouteButton->setEnabled(false);
    }

}
