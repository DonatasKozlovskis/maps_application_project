#ifndef POIWIDGET_H
#define POIWIDGET_H

#include <QWidget>
#include <QtWidgets>

#include "ui_poiwidget.h"

namespace Ui {
    class POIWidget;
}

class POIWidget : public QWidget {
    Q_OBJECT

public:
    explicit POIWidget(QWidget *parent = 0);

signals:

public slots:

private:
    Ui::POIWidget *p_ui;

};



#endif // POIWIDGET_H
