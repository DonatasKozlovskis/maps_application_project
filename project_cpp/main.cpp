#include "mapexplorer.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Some update by Ferran
    MapExplorer window;
    window.show();


    return a.exec();
}
