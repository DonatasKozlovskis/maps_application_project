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

protected:
    QSqlDatabase db;                        // database connection for the whole parent widget

};

#endif // MAPEXPLORER_H
