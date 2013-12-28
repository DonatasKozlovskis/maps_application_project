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
    void showPOIWidget();

    void on_driveRadioButton_clicked();

    void on_walkRadioButton_clicked();

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

    void initializeContextMenu();


protected:
    QSqlDatabase db;                        // database connection for the whole parent widget

};

#endif // MAPEXPLORER_H
