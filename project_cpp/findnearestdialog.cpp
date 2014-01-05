#include "findnearestdialog.h"
#include "ui_findnearestdialog.h"

/**
 * @brief FindNearestDialog::FindNearestDialog
 * Default constructor
 * @param parent
 */
FindNearestDialog::FindNearestDialog(QWidget *parent) : QDialog(parent), ui(new Ui::FindNearestDialog) {
    ui->setupUi(this);
    ui->maxDistanceCheckBox->setChecked(false);
    ui->distanceSpinBox->setEnabled(false);
    ui->maxDistanceCheckBox->setEnabled(false);
}

/**
 * @brief FindNearestDialog::~FindNearestDialog
 * Default destructor.
 */
FindNearestDialog::~FindNearestDialog()
{
    delete ui;
}

/**
 * @brief FindNearestDialog::on_cancelButton_clicked
 * Cancel button action - close the widget.
 */
void FindNearestDialog::on_cancelButton_clicked()
{
    this->close();
}

/**
 * @brief FindNearestDialog::setContainer
 * Mutator of the property container.
 * @param newContainer
 */
void FindNearestDialog::setContainer(POIContainer *newContainer) {
    container = newContainer;
    initializeComboBox();
}

/**
 * @brief FindNearestDialog::initializeComboBox
 * {@link QComboBox} initialization function.
 */
void FindNearestDialog::initializeComboBox() {
    QMap <int, QString> poiTypes = container->getTypeList();
    QMap <int, QString>::iterator i;

    for (i = poiTypes.begin(); i != poiTypes.end(); i++)
        ui->poiTypesComboBox->addItem(i.value(), (QVariant)i.key());

}

/**
 * @brief FindNearestDialog::on_findButton_clicked
 * Find butto function. Call another function to find nearest POI.
 */
void FindNearestDialog::on_findButton_clicked()
{
    findNearestPOI();
}

/**
 * @brief FindNearestDialog::on_maxDistanceCheckBox_toggled
 * {@link QCheckBox} toggle function. Enable / Disable {@link QSpinBox} responsible for distance.
 * @param checked
 */
void FindNearestDialog::on_maxDistanceCheckBox_toggled(bool checked)
{
    if (checked)
        ui->distanceSpinBox->setEnabled(true);
     else
        ui->distanceSpinBox->setEnabled(false);
}

/**
 * @brief FindNearestDialog::setPossiblePOICoords
 * Mutator of the possible POI coordiates.
 * @param newPos
 */
void FindNearestDialog::setPossiblePOICoords(QPointF newPos) {
    possiblePOICoords = newPos;
}

/**
 * @brief FindNearestDialog::findNearestPOI
 * Find nearest POI function.
 *
 * Queries the database for the specific type POIs and finds the one with the least distance (Euclidean).
 */
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
