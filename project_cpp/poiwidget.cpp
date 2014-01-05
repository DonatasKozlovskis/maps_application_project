#include <QMessageBox>
#include "poiwidget.h"

/**
 * @brief POIWidget::POIWidget
 * Deafult constructor.
 * @param parent
 */
POIWidget::POIWidget(QWidget *parent) : QWidget(parent), ui(new Ui::POIWidget) {
    ui->setupUi(this);
}

/**
 * @brief POIWidget::setContainer
 * Mutator of the private instance of the pointer to the {@link POIContainer} class
 * @param ncontainer - a new pointer to the container
 */
void POIWidget::setContainer(POIContainer* ncontainer) {

    container = ncontainer;
    initializeComboBoxes();
}

/**
 * @brief POIWidget::browsing
 * This function is used to prepare the widget for browsing.
 *
 * It is not possible to click the radiobutton externally, because the UI instance
 * is private
 */
void POIWidget::browsing() {
    ui->browseRadioButton->click();
}

/**
 * @brief POIWidget::editing
 * This function is used to prepare the wiget for editing.
 *
 * It is not possible to click the radiobutton externally, because the UI instance
 * is private
 */
void POIWidget::editing() {
    ui->editRadioButton->click();
}

// This is used to prepare widget for adding new POI
/**
 * @brief POIWidget::adding
 * This function is used to prepare the widget for browsing.
 *
 * It is not possible to click the radiobutton externally, because the UI instance
 * is private
 */
void POIWidget::adding() {
    ui->newRadioButton->click();
}

/**
 * @brief POIWidget::on_browseRadioButton_clicked
 * Disable all editable and edit-related widgets when user just browses through existing data.
 * Enable all browse-related widgets.
 */
void POIWidget::on_browseRadioButton_clicked()
{
    // Disable all edit-related widgets
    ui->poiNameLineEdit->setEnabled(false);
    ui->poiImageBrowseButton->setEnabled(false);
    ui->poiTypeComboBox->setEnabled(false);
    ui->addressTextEdit->setEnabled(false);
    ui->poiImagePathLineEdit->setEnabled(false);

    // Enable browsing widgets
    ui->poiComboBox->setEnabled(true);
    ui->poiTypeViewComboBox->setEnabled(true);

    // Update the fields with the values of the currently selected POI
    updatePOIFields();

    // The save and delete buttons are disabled in the end because the previous
    // changes would trigger them to be enabled.
    ui->saveButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->deleteButton->setText("Delete");
}

// Edit radio button checked - enable all editable fields
/**
 * @brief POIWidget::on_editRadioButton_clicked
 * Enable editable fields and other widgets.
 */
void POIWidget::on_editRadioButton_clicked()
{
    // Enable the editable fields
    ui->poiNameLineEdit->setEnabled(true);
    ui->poiImageBrowseButton->setEnabled(true);
    ui->poiTypeComboBox->setEnabled(true);
    ui->addressTextEdit->setEnabled(true);
    ui->poiImagePathLineEdit->setEnabled(true);
    ui->poiComboBox->setEnabled(true);
    ui->poiTypeViewComboBox->setEnabled(true);

    // Update the fields with the values of the currently selected POI
    updatePOIFields();

    // Disable the save button (no changes initially)
    ui->saveButton->setEnabled(false);
    ui->deleteButton->setText("Delete");
    ui->deleteButton->setEnabled(true);
}

/**
 * @brief POIWidget::on_newRadioButton_clicked
 * Clean all editable fields, ask user to pick the points from the map if necessary.
 */
void POIWidget::on_newRadioButton_clicked()
{
    // Enable editable fields
    ui->poiNameLineEdit->setEnabled(true);
    ui->poiImageBrowseButton->setEnabled(true);
    ui->poiTypeComboBox->setEnabled(true);
    ui->addressTextEdit->setEnabled(true);
    ui->poiImagePathLineEdit->setEnabled(true);

    // Clear editable fields
    ui->poiNameLineEdit->clear();
    ui->poiTypeComboBox->setCurrentIndex(0);
    ui->addressTextEdit->clear();
    ui->poiImagePathLineEdit->clear();

    // Disable POI selection combo boxes
    ui->poiComboBox->setEnabled(false);
    ui->poiTypeViewComboBox->setEnabled(false);

    // Change the "Delete" button text to "Reset"
    ui->deleteButton->setEnabled(false);
    ui->deleteButton->setText("Reset");

    ui->saveButton->setEnabled(true);

    // Ask user to pick the poit from the map
    if (possiblePOICoords.x() == -1) {
        QMessageBox::information(this,"User action required!",
                                 "Please right-click the point on the map \nwhere you want to locate the point of interest.");
    }

    // Update latitude and lomgitude labels
    ui->latLabel->setText(QString::number(possiblePOICoords.x()));
    ui->lonLabel->setText(QString::number(possiblePOICoords.y()));

}

/**
 * @brief POIWidget::initializeComboBoxes
 * When creating the widget for the first time, populate the type combo box with the POI types currently available.
 */
void POIWidget::initializeComboBoxes() {

    // '0' type is used to display everything
    ui->poiTypeViewComboBox->addItem("All",QVariant(0));
    // Get the POI types
    QMap <int, QString> poiTypes = container->getTypeList();
    QMap <int, QString> :: iterator i;

    // Populate the ComboBoxes
    for (i = poiTypes.begin(); i!= poiTypes.end(); i++) {
        ui->poiTypeViewComboBox->addItem(i.value(),(QVariant)i.key());
        ui->poiTypeComboBox->addItem(i.value(),(QVariant)i.key());
    }

    updatePOIComboBox();
}

/**
 * @brief POIWidget::updatePOIComboBox
 * Updates the POI combo box according to the POI type selected in the poi type combo box.
 */
void POIWidget::updatePOIComboBox() {
    int typeSelected = ui->poiTypeViewComboBox->itemData(ui->poiTypeViewComboBox->currentIndex()).toInt();

    // Delete previously entered values
    qDeleteAll(currentComboBoxPOIs);
    currentComboBoxPOIs.clear();
    ui->poiComboBox->clear();

    // Request new POIs
    currentComboBoxPOIs = container->getPOITypeFiltered(typeSelected);

    QMap <int, POI*>::iterator i;

    for (i = currentComboBoxPOIs.begin(); i != currentComboBoxPOIs.end(); i++)
        ui->poiComboBox->addItem( (*i)->getName(), QVariant((*i)->getId()));
}

/**
 * @brief POIWidget::on_poiTypeViewComboBox_currentIndexChanged
 * When the type of the POI changed inside the corresponding combo box,
 * updates the POIs displayed in the POI comboBox
 * @param arg1 - index of the element currently selected
 */
void POIWidget::on_poiTypeViewComboBox_currentIndexChanged(const QString &arg1)
{
    updatePOIComboBox();
}

/**
 * @brief POIWidget::on_poiComboBox_currentIndexChanged
 * When the index of the current POI selected is changed, update th values displayed on the fields of the widget
 *
 * @param index
 */
void POIWidget::on_poiComboBox_currentIndexChanged(int index)
{
    // In this case, index is just an index in the list.
    // Because we used addItem (QString name, poiId),
    // We ca get the ID of the POI from the selected field
    int poiIndex = ui->poiComboBox->itemData(index).toInt();
    poiSelected = container->getPOI(poiIndex);
    updatePOIFields();
    ui->saveButton->setEnabled(false);
}

/**
 * @brief POIWidget::on_poiNameLineEdit_textEdited
 * When user modifies the text in the field, enable the "Save" button
 *
 * @param arg1 - newly edited text
 */
void POIWidget::on_poiNameLineEdit_textEdited(const QString &arg1)
{
    ui->saveButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}

/**
 * @brief POIWidget::on_poiTypeComboBox_currentIndexChanged
 * When user changes the selection in the combo box, enable the "Save" button
 *
 * @param index - index of the chaged selection
 */
void POIWidget::on_poiTypeComboBox_currentIndexChanged(int index)
{
    ui->saveButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}

/**
 * @brief POIWidget::on_addressTextEdit_textChanged
 * When user modifies the text in the field, enable the "Save" button
 */
void POIWidget::on_addressTextEdit_textChanged()
{
    ui->saveButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}

/**
 * @brief POIWidget::on_poiImagePathLineEdit_textChanged
 * When current path is chaged, enable the "Save" button
 *
 * @param arg1 - newly edited string
 */
void POIWidget::on_poiImagePathLineEdit_textChanged(const QString &arg1)
{
    ui->saveButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}

/**
 * @brief POIWidget::updatePOIFields
 * Updates POI fields according to the information stored in the poiSelected variable
 */
void POIWidget::updatePOIFields() {
    if (poiSelected != NULL) {

        ui->poiNameLineEdit->setText(poiSelected->getName());
        ui->addressTextEdit->setText(poiSelected->getAddress());
        ui->poiTypeComboBox->setCurrentText(poiSelected->getType());
        ui->poiImagePathLineEdit->setText(poiSelected->getImgPath());

        ui->latLabel->setText(QString::number(poiSelected->getPoint().x));
        ui->lonLabel->setText(QString::number(poiSelected->getPoint().y));

        if (!ui->editRadioButton->isChecked())
            ui->deleteButton->setEnabled(false);
    }
}

/**
 * @brief POIWidget::on_cancelButton_clicked
 * Cancel button function. If any changes were made, ask user if he want to change them.
 */
void POIWidget::on_cancelButton_clicked()
{
    if (!ui->saveButton->isEnabled())
        this->close();
    else {

        // http://stackoverflow.com/questions/13111669/yes-no-message-box-using-qmessagebox
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Changes", "Do you want to discard all changes applied?",
                                                                 QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
            this->close();
    }
}

/**
 * @brief POIWidget::on_saveButton_clicked
 * Save button function. Modifies or creates a new variable depending on the actions taken before.
 */
void POIWidget::on_saveButton_clicked()
{
    // If the POI is being edited, update it
    if (ui->editRadioButton->isChecked()) {

        if ( container->modifyPOI(poiSelected->getId(),
                             ui->poiNameLineEdit->text(),
                             ui->poiTypeComboBox->itemData(ui->poiTypeComboBox->currentIndex()).toInt(),
                             ui->poiImagePathLineEdit->text(),
                             ui->addressTextEdit->toPlainText()) ) {

            QMessageBox::StandardButton sucModMessageBox =
                    QMessageBox::information(this,"Success","POI Modified Successfully!");

            updatePOIComboBox();
            ui->saveButton->setEnabled(false);

        } else {

            QMessageBox::StandardButton errModMessageBox =
                    QMessageBox::critical(this,"Error","Failed to modify POI! \nDatabase Error!");
        }

    // If the new POI is created
    } else if (ui->newRadioButton->isChecked()) {
        // If no coordinates were picked before
        if (possiblePOICoords.x() == -1) {
            QMessageBox::information(this,"User action required!",
                                     "Please right-click the point on the map \nwhere you want to locate the point of interest.");

            ui->latLabel->setText(QString::number(possiblePOICoords.x()));
            ui->lonLabel->setText(QString::number(possiblePOICoords.y()));

        } else {

            if (ui->poiNameLineEdit->text().isEmpty() ||
                    ui->addressTextEdit->toPlainText().isEmpty())
                QMessageBox::warning(this,"Error","Name and Address fields of the POI should not be empty!");
            else if (ui->poiNameLineEdit->text().length() > 199) {
                QMessageBox::warning(this,"Error","The length of the name should not exceed 200 characters!");
            } else if (ui->addressTextEdit->toPlainText().length() > 49) {
                QMessageBox::warning(this,"Error","The length of the address should not exceed 50 characters!");
            } else {

                if (container->addPOI((float)possiblePOICoords.x(),
                                  (float)possiblePOICoords.y(),
                                  ui->poiNameLineEdit->text(),
                                  ui->poiTypeComboBox->itemData(ui->poiTypeComboBox->currentIndex()).toInt(),
                                  QString(""),
                                  ui->addressTextEdit->toPlainText())) {
                    QMessageBox::information(this,"Success","Point of interest added successfully!");
                    updatePOIComboBox();
                    ui->browseRadioButton->click();

                    // Reset the possible coordinattes when a new POI is added.
                    possiblePOICoords.setX(-1);
                    possiblePOICoords.setY(-1);
                } else
                    QMessageBox::critical(this,"Error","Failed to add Point of interest!");
            }
        }
    }
}

/**
 * @brief POIWidget::setPossiblePOICoords
 * Mutator of the possible POI Coordinates variable
 * @param newPos - a QPointF containing latitude and longitude
 */
void POIWidget::setPossiblePOICoords(QPointF newPos) {
    possiblePOICoords = newPos;
}

/**
 * @brief POIWidget::on_deleteButton_clicked
 * Delete button function.
 * Button acts as "Delete" in the edit mode,
 * and "Reset" of all editable fields when adding new POI
 */
void POIWidget::on_deleteButton_clicked()
{
    if (ui->newRadioButton->isChecked()) {
        ui->poiNameLineEdit->clear();
        ui->poiTypeComboBox->setCurrentIndex(0);
        ui->addressTextEdit->clear();
        ui->poiImagePathLineEdit->clear();

        ui->deleteButton->setEnabled(false);
        ui->saveButton->setEnabled(false);

    } else if (ui->editRadioButton->isChecked()) {
        if (container->removePOI(poiSelected->getId())) {
            QMessageBox::information(this,"Success","Point of interest removed successfully!");
            updatePOIComboBox();
        }
    }
 }
