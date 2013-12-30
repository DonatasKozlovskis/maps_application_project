#ifndef POIWIDGET_H
#define POIWIDGET_H

#include <QWidget>
#include <QtWidgets>

#include "ui_poiwidget.h"
#include "poicontainer.h"

namespace Ui {
    class POIWidget;
}

class POIWidget : public QWidget {
    Q_OBJECT

    friend class MapGLWidget;

public:
    explicit POIWidget(QWidget *parent = 0);
    void setContainer (POIContainer*);

    void browsing();
    void editing();
    void adding();

signals:

public slots:

private:
    Ui::POIWidget *ui;

    void initializeComboBoxes();
    void updatePOIComboBox();
    void updatePOIFields();

    QMap <int, POI*> currentComboBoxPOIs;

protected:
    POIContainer* container;
    POI* poiSelected;

private slots:
    void on_browseRadioButton_clicked();
    void on_editRadioButton_clicked();
    void on_newRadioButton_clicked();
    void on_poiTypeViewComboBox_currentIndexChanged(const QString &arg1);
    void on_poiComboBox_currentIndexChanged(int index);
    void on_poiNameLineEdit_textEdited(const QString &arg1);
    void on_poiTypeComboBox_currentIndexChanged(int index);
    void on_addressTextEdit_textChanged();
    void on_poiImagePathLineEdit_textChanged(const QString &arg1);
    void on_cancelButton_clicked();
    void on_saveButton_clicked();
};



#endif // POIWIDGET_H
