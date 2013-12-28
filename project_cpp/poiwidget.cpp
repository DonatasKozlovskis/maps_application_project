#include "poiwidget.h"

// Class represents the widget for browsing or modifying POI data
POIWidget::POIWidget(QWidget *parent) : QWidget(parent), p_ui(new Ui::POIWidget) {
    p_ui->setupUi(this);
}


