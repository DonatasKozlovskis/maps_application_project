#ifndef MAPEXPLORER_H
#define MAPEXPLORER_H

#include <QMainWindow>
#include <QtSql>

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
    void updateMapPos(int,int);
    void updateMapScale(float);

private:
    // User interface
    Ui::MapExplorer *ui;

    // Database stuff
    void connectDatabase();
    void disconnectDatabase();

    // Even the map movement is integer, it should b float i order not to use casting
    // while calculating. Setting it to QPoint (integer) produces precision error when
    // obtaining map coordinates
    QPointF mapMoved;
    // Current map scale
    float mapScale;

protected:
    QSqlDatabase db;                        // database connection for the whole parent widget

};

#endif // MAPEXPLORER_H
