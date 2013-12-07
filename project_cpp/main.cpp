#include <QApplication>
#include <QTableWidget>
#include <qmessagebox.h>

#include "mainwindow.h"
#include "glwidget.h"
#include "map.h"

using namespace std;

void connectDataBase();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTableWidget* table = new QTableWidget();
    table->setWindowTitle("Connect to Mysql Database Example");

    connectDataBase();

    /*QSqlQuery query("SELECT  * FROM road;");
    table->setColumnCount(query.record().count());
    table->setRowCount(query.size());

    int index=0;

    while (query.next())
    {
    table->setItem(index,0,new QTableWidgetItem(query.value(0).toString()));
    table->setItem(index,1,new QTableWidgetItem(query.value(1).toString()));
    table->setItem(index,2,new QTableWidgetItem(query.value(2).toString()));
    index++;
    }

    table->show();*/

    MainWindow w;
    GLWidget *gl = new GLWidget;

    w.setCentralWidget(gl);
    w.resize(1280,720);
    w.move(200,200);
    w.show();

    return a.exec();
}

void connectDataBase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("schneider");
    db.setUserName("root");
    db.setPassword("root");
    if (!db.open())
    {
      QMessageBox::critical(0, QObject::tr("Database Error"),
                db.lastError().text());
    }
}
