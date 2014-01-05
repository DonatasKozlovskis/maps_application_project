#include "findnearestdialog.h"
#include "ui_findnearestdialog.h"

FindNearestDialog::FindNearestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindNearestDialog)
{
    ui->setupUi(this);
    ui->maxDistanceCheckBox->setChecked(false);
    ui->distanceSpinBox->setEnabled(false);

}

FindNearestDialog::~FindNearestDialog()
{
    delete ui;
}

void FindNearestDialog::on_cancelButton_clicked()
{
    this->close();
}

void FindNearestDialog::setContainer(POIContainer *newContainer) {
    container = newContainer;
    initializeComboBox();
}

void FindNearestDialog::initializeComboBox() {
    QMap <int, QString> poiTypes = container->getTypeList();
    QMap <int, QString>::iterator i;

    for (i = poiTypes.begin(); i != poiTypes.end(); i++)
        ui->poiTypesComboBox->addItem(i.value(), (QVariant)i.key());

}

void FindNearestDialog::on_findButton_clicked()
{
    findNearestPOI();
}

void FindNearestDialog::on_maxDistanceCheckBox_toggled(bool checked)
{
    if (checked)
        ui->distanceSpinBox->setEnabled(true);
     else
        ui->distanceSpinBox->setEnabled(false);
}

void FindNearestDialog::setPossiblePOICoords(QPointF newPos) {
    possiblePOICoords = newPos;
}

void FindNearestDialog::findNearestPOI() {
    QMap <int, POI*> poisFiltered = container->getPOITypeFiltered(
                ui->poiTypesComboBox->itemData(ui->poiTypesComboBox->currentIndex()).toInt());

    QMap <int, POI*>::iterator i;

    cout << poisFiltered.size() << endl;

    float maxDistance = 9999;
    POI* point = NULL;

    cout << "Possible POI: X: " << possiblePOICoords.x() <<
            " Y: " << possiblePOICoords.y() << endl;

    for (i = poisFiltered.begin(); i != poisFiltered.end(); i++) {

        float temp = sqrt(pow(possiblePOICoords.x() - (*i)->getPoint().x,2) +
                        pow(possiblePOICoords.y() - (*i)->getPoint().y,2));

        cout << "Current: X: " << (*i)->getPoint().x << " Y:" << (*i)->getPoint().y << endl;

        if (temp < maxDistance) {
            maxDistance = temp;
            point = (*i);
        }

        cout << "Distance: " << maxDistance << endl;
    }

    if (point) {
        cout << point->getName().toStdString() << endl;
        emit nearestPOIFound(possiblePOICoords,point->getId());
    }
    qDeleteAll(poisFiltered);
}
