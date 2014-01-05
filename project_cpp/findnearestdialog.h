#ifndef FINDNEARESTDIALOG_H
#define FINDNEARESTDIALOG_H

#include <QDialog>

#include "poicontainer.h"

namespace Ui {
class FindNearestDialog;
}

class FindNearestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindNearestDialog(QWidget *parent = 0);
    ~FindNearestDialog();

    void setContainer(POIContainer*);
    void setPossiblePOICoords(QPointF);

private slots:
    void on_cancelButton_clicked();

    void on_findButton_clicked();

    void on_maxDistanceCheckBox_toggled(bool checked);

signals:
    void nearestPOIFound(QPointF, int);

private:
    Ui::FindNearestDialog *ui;

    void initializeComboBox();
    void findNearestPOI();

    QPointF possiblePOICoords;                  /** Contains the possible coordinates of the new POI.*/

protected:
    POIContainer* container;                    /** A pointer to the {@link POIContainer} class.*/

};

#endif // FINDNEARESTDIALOG_H
