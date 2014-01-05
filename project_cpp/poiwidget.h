#ifndef POIWIDGET_H
#define POIWIDGET_H

#include <QWidget>
#include <QtWidgets>

#include "ui_poiwidget.h"
#include "poicontainer.h"

namespace Ui {
    class POIWidget;
}

/**
 * @brief The POIWidget class
 * This widget is used tho perform various actions on the points of interest on the map.
 */
class POIWidget : public QWidget {
    Q_OBJECT

    friend class MapGLWidget;

public:
    explicit POIWidget(QWidget *parent = 0);
    void setContainer (POIContainer*);

    void browsing();
    void editing();
    void adding();

    void setPossiblePOICoords (QPointF);

signals:

public slots:

private:
    Ui::POIWidget *ui;      /** A pointer to the UI instance. */

    void initializeComboBoxes();
    void updatePOIComboBox();
    void updatePOIFields();

    // Do I really need this variable?
    QMap <int, POI*> currentComboBoxPOIs;       /** Contains the current comboBox POIs that are currently displayed.*/
    // I've found no other normal way to store it.
    QPointF possiblePOICoords;                  /** Contains the possible coordinates of the new POI.*/

protected:
    POIContainer* container;                    /** A pointer to the {@link POIContainer} class. */
    POI* poiSelected;                           /** Stores the pointer to the currently selected POI. */

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
    void on_deleteButton_clicked();
};

#endif // POIWIDGET_H
