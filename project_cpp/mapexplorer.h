#ifndef MAPEXPLORER_H
#define MAPEXPLORER_H

#include <QMainWindow>
#include <QtSql>
#include "poiwidget.h"

#include "ui_mapexplorer.h"

/**
 * Namespace used for the UI.
 */
namespace Ui {
    class MapExplorer;
}

/**
 * @brief The MapExplorer class
 * The MapExplorer class is derived from the MainWindow and represents the main windows of the program.
 */
class MapExplorer : public QMainWindow {
    Q_OBJECT

public:
    explicit MapExplorer(QWidget *parent = 0);
    ~MapExplorer();

private slots:
    // Exit & About
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    // Context menu
    void on_mapGLWidget_customContextMenuRequested(const QPoint &pos);
    // Drive/Walk radio buttons
    void on_driveRadioButton_clicked();
    void on_walkRadioButton_clicked();
    // POIWdiget
    void on_managePOIButton_clicked();
    void showPOIWidgetNew();
    // Combo boxes
    void on_P1TypeComboBox_currentIndexChanged(int);
    void on_P2TypeComboBox_currentIndexChanged(int);
    void on_P3TypeComboBox_currentIndexChanged(int);
    void on_poiDisplayComboBox_currentIndexChanged(int);
    void on_P1ComboBox_currentIndexChanged(int index);
    void on_P2ComboBox_currentIndexChanged(int index);
    void on_P3ComboBox_currentIndexChanged(int index);
    // POI deletion buttons
    void on_deleteP1Button_clicked();
    void on_deleteP2Button_clicked();
    void on_deleteP3Button_clicked();

    void updateDebugInfo(int, int);

private:
    // User interface
    Ui::MapExplorer *ui;                        /** A pointer to the UI instance. (Singleton) */
    POIWidget* poiwidget;                       /** A pointer to the {@link POIWidget}. (Singleton) */

    // Context menu
    QMenu* contextMenu;                         /** Custom context menu called in the {@MapGLWidget}. */
    QAction* newPOIAct;                         /** Action of the context menu responsible for calling the {@link POIWidget} */
    QAction* findNearestAct;                    /** Action responsible for the finding nearest POI. */

    QComboBox* poiComboBoxes[3];                /** An array of the pointers to the {@link QComboBox}*/

    // Database stuff
    void connectDatabase();
    void disconnectDatabase();

    // Some widget update functions
    void initializeContextMenu();
    void initializeComboBoxes();

    // ComboBoxes update functions
    void updateP1ComboBox();
    void updateP2ComboBox();
    void updateP3ComboBox();
    void updateComboBox(int,int);

protected:
    QSqlDatabase db;                        /** A QSqlDatabase instance that is responsible for the database connection. */

    // Not good
    POIContainer* container;                    /** A pointer to the POI container stored in the {@link MapGlWidget}. */
    Node currentPOINode;                        /** Coordinates of the possible POI candidate. */
    QMap <int, POI*> currentComboBoxPOIs[3];    /** An array of {@link QMap} containing the POIs located in the corresponding QComboBox. */
    // Redundant. Can be derived from the ComboBox current index.
    POI* currentPOIs[3];                        /** Three current POIs selected */
};

#endif // MAPEXPLORER_H
