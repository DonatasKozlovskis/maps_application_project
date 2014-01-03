#ifndef MAPEXPLORER_H
#define MAPEXPLORER_H

#include <QMainWindow>
#include <QtSql>
#include "poiwidget.h"

#include "ui_mapexplorer.h"


namespace Ui {
    class MapExplorer;
}

class MapExplorer : public QMainWindow {
    Q_OBJECT

public:
    explicit MapExplorer(QWidget *parent = 0);
    ~MapExplorer();

private slots:
    // Exit & About
    void on_actionExit_triggered();
    void on_actionAbout_triggered();

    // Debug output 2
    void updateDebugInfo(int, int);
    void on_mapGLWidget_customContextMenuRequested(const QPoint &pos);
    void showPOIWidgetNew();

    void on_driveRadioButton_clicked();
    void on_walkRadioButton_clicked();
    void on_managePOIButton_clicked();

    void on_P1TypeComboBox_currentIndexChanged(int);
    void on_P2TypeComboBox_currentIndexChanged(int);
    void on_P3TypeComboBox_currentIndexChanged(int);

    void on_poiDisplayComboBox_currentIndexChanged(int);

    void on_P1ComboBox_currentIndexChanged(int index);
    void on_P2ComboBox_currentIndexChanged(int index);
    void on_P3ComboBox_currentIndexChanged(int index);

    void on_deleteP1Button_clicked();

    void on_deleteP2Button_clicked();

    void on_deleteP3Button_clicked();

private:
    // User interface
    Ui::MapExplorer *ui;
    POIWidget* poiwidget;

    // Database stuff
    void connectDatabase();
    void disconnectDatabase();

    // Context menu
    QMenu* contextMenu;
    QAction* newPOIAct;
    QAction* findNearestAct;

    QMap <int, POI*> currentComboBoxPOIs[3];
    QComboBox* poiComboBoxes[3];

    POIContainer* container;        // Not good
    Node currentPOINode;

    POI* currentPOIs[3];

    void initializeContextMenu();
    void initializeComboBoxes();

    void updateP1ComboBox();
    void updateP2ComboBox();
    void updateP3ComboBox();

    void updateComboBox(int,int);





protected:
    QSqlDatabase db;                        // database connection for the whole parent widget

};

#endif // MAPEXPLORER_H
