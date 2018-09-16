#include "DriveTypeDialog.h"
#include "ui_DriveTypeDialog.h"

DriveTypeDialog::DriveTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DriveTypeDialog)
{
    ui->setupUi(this);

    QStringList supports;
    supports.append(DriveType::COMPUTER);
    supports.append(DriveType::EX_HARD_DRIVE);
    supports.append(DriveType::SMARTPHONE);
    ui->cb_drivesupport->addItems(supports);
}

QString DriveTypeDialog::getSelectedSupport()
{
    return ui->cb_drivesupport->currentText();
}

DriveTypeDialog::~DriveTypeDialog()
{
    delete ui;
}
