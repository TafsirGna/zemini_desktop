#include "UploadingDialog.h"
#include "ui_UploadingDialog.h"

UploadingDialog::UploadingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadingDialog)
{
    ui->setupUi(this);
}

UploadingDialog::~UploadingDialog()
{
    delete ui;
}
