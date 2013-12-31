#include <QMessageBox>
#include "poiwidget.h"

// Class represents the widget for browsing or modifying POI data
POIWidget::POIWidget(QWidget *parent) : QWidget(parent), ui(new Ui::POIWidget) {
    ui->setupUi(this);
}

// Mutator of the POI container
void POIWidget::setContainer(POIContainer* ncontainer) {
    container = ncontainer;
    initializeComboBoxes();
}

// This is used to prepare widget for browsing
void POIWidget::browsing() {
    ui->browseRadioButton->click();
}

// This is used to prepare widget for editing
void POIWidget::editing() {
    ui->editRadioButton->click();
}

// This is used to prepare widget for adding new POI
void POIWidget::adding() {
    ui->newRadioButton->click();
}

// Browse radio button checked - disable all editable fields
void POIWidget::on_browseRadioButton_clicked()
{
    ui->poiNameLineEdit->setEnabled(false);
    ui->poiImageBrowseButton->setEnabled(false);
    ui->poiTypeComboBox->setEnabled(false);
    ui->addressTextEdit->setEnabled(false);
    ui->poiImagePathLineEdit->setEnabled(false);
    ui->poiComboBox->setEnabled(true);
    ui->poiTypeViewComboBox->setEnabled(true);

    updatePOIFields();

    ui->saveButton->setEnabled(false);
}

// Edit radio button checked - enable all editable fields
void POIWidget::on_editRadioButton_clicked()
{
    ui->poiNameLineEdit->setEnabled(true);
    ui->poiImageBrowseButton->setEnabled(true);
    ui->poiTypeComboBox->setEnabled(true);
    ui->addressTextEdit->setEnabled(true);
    ui->poiImagePathLineEdit->setEnabled(true);
    ui->poiComboBox->setEnabled(true);
    ui->poiTypeViewComboBox->setEnabled(true);

    updatePOIFields();

    ui->saveButton->setEnabled(false);
}

// New radio button clicked
void POIWidget::on_newRadioButton_clicked()
{
    ui->poiNameLineEdit->setEnabled(true);
    ui->poiImageBrowseButton->setEnabled(true);
    ui->poiTypeComboBox->setEnabled(true);
    ui->addressTextEdit->setEnabled(true);
    ui->poiImagePathLineEdit->setEnabled(true);

    ui->poiNameLineEdit->clear();
    ui->poiTypeComboBox->setCurrentIndex(0);
    ui->addressTextEdit->clear();
    ui->poiImagePathLineEdit->clear();

    ui->poiComboBox->setEnabled(false);
    ui->poiTypeViewComboBox->setEnabled(false);

    ui->saveButton->setEnabled(false);

}

void POIWidget::initializeComboBoxes() {

    QMap <int, QString> poiTypes = container->getTypeList();

    ui->poiTypeViewComboBox->addItem("All",QVariant(0));

    QMap <int, QString> :: iterator i;

    for (i = poiTypes.begin(); i!= poiTypes.end(); i++) {
        ui->poiTypeViewComboBox->addItem(i.value(),(QVariant)i.key());
        ui->poiTypeComboBox->addItem(i.value(),(QVariant)i.key());
    }

    updatePOIComboBox();
}

// Updates the POI combo box.
// First, it cleans the global variable responsible for handling all current combo boxes
// Then, it cleans the combobox items
// After both are updated
void POIWidget::updatePOIComboBox() {
    int typeSelected = ui->poiTypeViewComboBox->itemData(ui->poiTypeViewComboBox->currentIndex()).toInt();

    // Delete previously entered values
    qDeleteAll(currentComboBoxPOIs);
    ui->poiComboBox->clear();

    currentComboBoxPOIs = container->getPOITypeFiltered(typeSelected);

    QMap <int, POI*>::iterator i;

    for (i = currentComboBoxPOIs.begin(); i != currentComboBoxPOIs.end(); i++)
        ui->poiComboBox->addItem( (*i)->getName(), QVariant((*i)->getId()));

}

// When the type of the POI changed inside the corresponding combo box,
// update the POIs displayed in the POI comboBox
void POIWidget::on_poiTypeViewComboBox_currentIndexChanged(const QString &arg1)
{
    updatePOIComboBox();
}

// When the index of the current POI selected is changed, update th values displayed on the fields
// of the widget
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

// When user modifies the text in the field, enable the "Save" button
void POIWidget::on_poiNameLineEdit_textEdited(const QString &arg1)
{
    ui->saveButton->setEnabled(true);
}

// When user changes the selection in the combo box, enable the "Save" button
void POIWidget::on_poiTypeComboBox_currentIndexChanged(int index)
{
    ui->saveButton->setEnabled(true);
}

// When user modifies the text in the field, enable the "Save" button
void POIWidget::on_addressTextEdit_textChanged()
{
    ui->saveButton->setEnabled(true);
}

// When current path is chaged, enable the "Save" button
void POIWidget::on_poiImagePathLineEdit_textChanged(const QString &arg1)
{
    ui->saveButton->setEnabled(true);
}

// Updates POI fields according to the information stored in the poiSelected
// variable
void POIWidget::updatePOIFields() {
    if (poiSelected != NULL) {

        ui->poiNameLineEdit->setText(poiSelected->getName());
        ui->addressTextEdit->setText(poiSelected->getAddress());
        ui->poiTypeComboBox->setCurrentText(poiSelected->getType());
        ui->poiImagePathLineEdit->setText(poiSelected->getImgPath());
    }
}

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

void POIWidget::on_saveButton_clicked()
{
    if (ui->editRadioButton->isChecked()) {

        if ( container->modifyPOI(poiSelected->getId(),
                             ui->poiNameLineEdit->text(),
                             ui->poiTypeComboBox->currentText(),
                             ui->poiImagePathLineEdit->text(),
                             ui->addressTextEdit->toPlainText()) ) {

            QMessageBox::StandardButton sucModMessageBox =
                    QMessageBox::information(this,"Success","POI Modified Successfully!");
            //sucModMessageBox.exec();


            ui->saveButton->setEnabled(false);


        } else {

            QMessageBox::StandardButton errModMessageBox =
                    QMessageBox::critical(this,"Error","Failed to modify POI! \nDatabase Error!");
            //errModMessageBox.exec();

        }



    } else if (ui->newRadioButton->isChecked()) {
        // TODO:
        // Add new POI
    }
}
