#ifndef UPLOADINGDIALOG_H
#define UPLOADINGDIALOG_H

#include <QDialog>

namespace Ui {
class UploadingDialog;
}

class UploadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UploadingDialog(QWidget *parent = 0);
    ~UploadingDialog();

private:
    Ui::UploadingDialog *ui;
};

#endif // UPLOADINGDIALOG_H
