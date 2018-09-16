#ifndef DRIVETYPEDIALOG_H
#define DRIVETYPEDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <Database/Entities/DriveType.h>

namespace Ui {
class DriveTypeDialog;
}

class DriveTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DriveTypeDialog(QWidget *parent = 0);
    QString getSelectedSupport();
    ~DriveTypeDialog();

private:
    Ui::DriveTypeDialog *ui;
};

#endif // DRIVETYPEDIALOG_H
